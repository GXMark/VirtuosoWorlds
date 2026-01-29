#include "Region/VRegionClient.h"
#include "Presentation/VCollisionPresenter.h"
#include "Presentation/VDecalPresenter.h"
#include "Presentation/VLightPresenter.h"
#include "Presentation/VMaterialPresenter.h"
#include "Presentation/VMeshPresenter.h"
#include "Subsystem/VAssetManager.h"
#include "Region/VRegionClientBridge.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

static FORCEINLINE FTransform ToTransform(const FVMTransformNet& Net)
{
	FTransform Xf;
	Xf.SetLocation(FVector(Net.Location));
	Xf.SetRotation(Net.Rotation.ToQuat());
	Xf.SetScale3D(Net.bHasScale ? FVector(Net.Scale) : FVector(1.f, 1.f, 1.f));
	return Xf;
}

AVRegionClient::AVRegionClient()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void AVRegionClient::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
	{
		return;
	}

	PresentationRoot = NewObject<USceneComponent>(this, TEXT("PresentationRoot"));
	PresentationRoot->RegisterComponent();
	PresentationRoot->AttachToComponent(
		Root,
		FAttachmentTransformRules::KeepRelativeTransform);

	// Collision root (no visuals; holds per-item collision primitives)
	CollisionRoot = NewObject<USceneComponent>(this, TEXT("CollisionRoot"));
	CollisionRoot->RegisterComponent();
	CollisionRoot->AttachToComponent(
		Root,
		FAttachmentTransformRules::KeepRelativeTransform);

	AssetManager = GetWorld()->GetSubsystem<UVAssetManager>();

	MeshPresenter = NewObject<UVMeshPresenter>(this);
	MeshPresenter->Initialize(this, PresentationRoot, AssetManager);

	MaterialPresenter = NewObject<UVMaterialPresenter>(this);
	MaterialPresenter->Initialize(AssetManager);

	LightPresenter = NewObject<UVLightPresenter>(this);
	LightPresenter->Initialize(this, PresentationRoot);

	DecalPresenter = NewObject<UVDecalPresenter>(this);
	DecalPresenter->Initialize(this, PresentationRoot, AssetManager);

	CollisionPresenter = NewObject<UVCollisionPresenter>(this);
	CollisionPresenter->Initialize(this, CollisionRoot);

	// Initialize region bridge for all request types (spatial/material/collision).
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		RegionBridge = NewObject<URegionClientBridge>(this, TEXT("RegionBridge"));
		RegionBridge->Initialize(PC);
	}

	// Start streaming on the local client.
	bSpatialStreamActive = true;
	bSpatialHasMore = true;
	bSpatialRequestInFlight = false;

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* P = PC->GetPawn())
		{
			SpatialOrigin = P->GetActorLocation();
		}
	}
}

void AVRegionClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetNetMode() != NM_Client)
	{
		return;
	}

	if (!bSpatialStreamActive)
	{
		return;
	}

	// Determine current player position.
	FVector CurrentPos = SpatialOrigin;
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* P = PC->GetPawn())
		{
			CurrentPos = P->GetActorLocation();
		}
	}

	// Initialize stream origin on first tick.
	if (SpatialOrigin.IsNearlyZero())
	{
		SpatialOrigin = CurrentPos;
	}

	// Movement-based streaming: if the player moved far enough, recenter the stream.
	const float RecenterDistSq = FMath::Square(FMath::Max(0.f, SpatialRecenterDistanceCm));
	if (RecenterDistSq > 0.f && FVector::DistSquared(CurrentPos, SpatialOrigin) > RecenterDistSq)
	{
		// If a request is in-flight, defer the recenter until we get the response.
		bSpatialRecenterPending = bSpatialRequestInFlight;
		PendingSpatialOrigin = CurrentPos;
		if (!bSpatialRequestInFlight)
		{
			SpatialOrigin = CurrentPos;
			bSpatialHasMore = true;
		}
	}

	// If a request is in-flight, wait for the response before doing more.
	if (bSpatialRequestInFlight)
	{
		// Still allow material/collision pumps to progress while spatial is in-flight.
		PumpSpatialResolutionQueue();
		PumpMaterialQueue();
		PumpCollisionQueue();
		return;
	}

	// If the server told us there is no more data for the current stream, stop unless we recenter.
	if (!bSpatialHasMore)
	{
		// Spatial may be complete, but material/collision queues may still have work.
		PumpSpatialResolutionQueue();
		PumpMaterialQueue();
		PumpCollisionQueue();
		return;
	}

	RequestNextSpatialBatch();
	// Also pump dependent queues each tick.
	PumpSpatialResolutionQueue();
	PumpMaterialQueue();
	PumpCollisionQueue();
}

void AVRegionClient::PresentSpatialItemsBatch(const TArray<FVMSpatialItemNet>& Items)
{
	if (Items.Num() <= 0)
	{
		return;
	}

	PendingSpatialItems.Append(Items);
	PumpSpatialResolutionQueue();
}

void AVRegionClient::OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items, bool bHasMore)
{
	bSpatialRequestInFlight = false;
	bSpatialHasMore = bHasMore;

	// If we deferred a recenter while a request was in-flight, apply it now and restart the stream.
	if (bSpatialRecenterPending)
	{
		bSpatialRecenterPending = false;
		if (!PendingSpatialOrigin.IsNearlyZero())
		{
			SpatialOrigin = PendingSpatialOrigin;
		}
		PendingSpatialOrigin = FVector::ZeroVector;
		bSpatialHasMore = true;
	}

	PresentSpatialItemsBatch(Items);
	EnqueueDependenciesFromSpatialItems(Items);
	PumpSpatialResolutionQueue();
	PumpMaterialQueue();
	PumpCollisionQueue();
}

void AVRegionClient::SetSpatialStreamingEnabled(bool bEnabled)
{
	bSpatialStreamActive = bEnabled;
	if (bSpatialStreamActive)
	{
		bSpatialHasMore = true;
	}
}

void AVRegionClient::RequestNextSpatialBatch()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!RegionBridge || !RegionBridge->IsValidBridge())
	{
		UE_LOG(LogTemp, Error, TEXT("RegionClient: RegionBridge is not valid"));
		return;
	}

	bSpatialRequestInFlight = true;
	RegionBridge->RequestSpatialItems(SpatialOrigin, MaxSpatialItemsPerRequest);
}

void AVRegionClient::EnqueueDependenciesFromSpatialItems(const TArray<FVMSpatialItemNet>& Items)
{
	for (const FVMSpatialItemNet& Item : Items)
	{
		// Materials are only required for mesh payloads.
		if (Item.PayloadType == ESpatialItemType::Mesh)
		{
			for (const FVMGuidNet& MatNet : Item.MeshPayload.material_ids)
			{
				const FGuid MatId = MatNet.Value;
				if (!MatId.IsValid() || ReceivedMaterialIds.Contains(MatId) || InFlightMaterialIds.Contains(MatId))
				{
					continue;
				}
				PendingMaterialIds.Add(MatId);
			}
		}
		else if (Item.PayloadType == ESpatialItemType::Decal)
		{
			const FGuid MatId = Item.DecalPayload.material_id.Value;
			if (MatId.IsValid() && !ReceivedMaterialIds.Contains(MatId) && !InFlightMaterialIds.Contains(MatId))
			{
				PendingMaterialIds.Add(MatId);
			}
		}

		// Collisions now live at the spatial-item level.
		const FGuid ColId = Item.CollisionID.Value;
		if (ColId.IsValid() && !ReceivedCollisionIds.Contains(ColId) && !InFlightCollisionIds.Contains(ColId))
		{
			PendingCollisionIds.Add(ColId);
		}
	}
}

void AVRegionClient::PumpSpatialResolutionQueue()
{
	if (PendingSpatialItems.Num() <= 0)
	{
		return;
	}

	const int32 Count = FMath::Min(MaxSpatialItemsToResolvePerTick, PendingSpatialItems.Num());
	for (int32 Index = 0; Index < Count; ++Index)
	{
		ResolveSpatialItem(PendingSpatialItems[Index]);
	}
	PendingSpatialItems.RemoveAt(0, Count, EAllowShrinking::No);
}

void AVRegionClient::ResolveSpatialItem(const FVMSpatialItemNet& Item)
{
	const FGuid ItemId = Item.ItemID.Value;
	const FTransform ItemXf = ToTransform(Item.Transform);

	if (CollisionPresenter)
	{
		CollisionPresenter->OnItemUpsert(ItemId, ItemXf, Item.CollisionID.Value);
	}

	switch (Item.PayloadType)
	{
	case ESpatialItemType::Mesh:
	{
		UStaticMeshComponent* MeshComp = MeshPresenter
			? MeshPresenter->PresentMeshItem(
				ItemId,
				Item.MeshPayload,
				ItemXf,
				Item.ParentID.Value)
			: nullptr;

		if (MeshComp && MaterialPresenter)
		{
			MaterialPresenter->ApplyMaterialsAsync(ItemId, MeshComp, Item.MeshPayload.material_ids);
		}
		break;
	}
	case ESpatialItemType::PointLight:
		if (LightPresenter)
		{
			LightPresenter->PresentPointLightItem(ItemId, Item.PointLightPayload, ItemXf);
		}
		break;
	case ESpatialItemType::SpotLight:
		if (LightPresenter)
		{
			LightPresenter->PresentSpotLightItem(ItemId, Item.SpotLightPayload, ItemXf);
		}
		break;
	case ESpatialItemType::Decal:
		if (DecalPresenter)
		{
			DecalPresenter->PresentDecalItem(ItemId, Item.DecalPayload, ItemXf);
		}
		break;
	default:
		break;
	}
}

void AVRegionClient::PumpMaterialQueue()
{
	if (bMaterialRequestInFlight)
	{
		return;
	}
	if (!RegionBridge || !RegionBridge->IsValidBridge())
	{
		return;
	}
	if (PendingMaterialIds.Num() <= 0)
	{
		return;
	}

	const int32 Count = FMath::Min(MaxMaterialItemLimit, PendingMaterialIds.Num());
	TArray<FGuid> Batch;
	Batch.Reserve(Count);
	for (int32 i = 0; i < Count; ++i)
	{
		Batch.Add(PendingMaterialIds[i]);
		InFlightMaterialIds.Add(PendingMaterialIds[i]);
	}
	PendingMaterialIds.RemoveAt(0, Count, EAllowShrinking::No);
	bMaterialRequestInFlight = true;
	RegionBridge->RequestMaterialsBatch(Batch);
}

void AVRegionClient::PumpCollisionQueue()
{
	if (bCollisionRequestInFlight)
	{
		return;
	}
	if (!RegionBridge || !RegionBridge->IsValidBridge())
	{
		return;
	}
	if (PendingCollisionIds.Num() <= 0)
	{
		return;
	}

	const int32 Count = FMath::Min(MaxCollisionItemLimit, PendingCollisionIds.Num());
	TArray<FGuid> Batch;
	Batch.Reserve(Count);
	for (int32 i = 0; i < Count; ++i)
	{
		Batch.Add(PendingCollisionIds[i]);
		InFlightCollisionIds.Add(PendingCollisionIds[i]);
	}
	PendingCollisionIds.RemoveAt(0, Count, EAllowShrinking::No);
	bCollisionRequestInFlight = true;
	RegionBridge->RequestCollisionsBatch(Batch);
}

void AVRegionClient::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	bMaterialRequestInFlight = false;
	if (AssetManager)
	{
		AssetManager->SubmitMaterialItems(Materials);
	}
	for (const FVMMaterial& Mat : Materials)
	{
		ReceivedMaterialIds.Add(Mat.id);
		InFlightMaterialIds.Remove(Mat.id);
	}
	PumpMaterialQueue();
}

void AVRegionClient::OnCollisionsBatchReceived(const TArray<FVMCollision>& Collisions)
{
	bCollisionRequestInFlight = false;
	for (const FVMCollision& Col : Collisions)
	{
		// Assume collision id field is named 'id' in FVMCollision
		ReceivedCollisionIds.Add(Col.id);
		InFlightCollisionIds.Remove(Col.id);
	}

	if (CollisionPresenter)
	{
		CollisionPresenter->SubmitCollisionDefs(Collisions);
	}
	PumpCollisionQueue();
}
