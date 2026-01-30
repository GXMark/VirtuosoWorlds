#include "Region/VRegionClient.h"
#include "Presentation/VCollisionPresenter.h"
#include "Presentation/VDecalPresenter.h"
#include "Presentation/VLightPresenter.h"
#include "Presentation/VMaterialPresenter.h"
#include "Presentation/VMeshPresenter.h"
#include "Subsystem/VAssetManager.h"
#include "Region/VRegionPresenter.h"
#include "Region/VRegionResolver.h"
#include "Region/VRegionClientBridge.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

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
	MaterialPresenter->Initialize();

	LightPresenter = NewObject<UVLightPresenter>(this);
	LightPresenter->Initialize(this, PresentationRoot);

	DecalPresenter = NewObject<UVDecalPresenter>(this);
	DecalPresenter->Initialize(this, PresentationRoot);

	CollisionPresenter = NewObject<UVCollisionPresenter>(this);
	CollisionPresenter->Initialize(this, CollisionRoot);

	// Initialize region bridge for all request types (spatial/material/collision).
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		RegionBridge = NewObject<URegionClientBridge>(this, TEXT("RegionBridge"));
		RegionBridge->Initialize(PC);
		CachedPawn = PC->GetPawn();
	}

	RegionResolver = NewObject<UVRegionResolver>(this);
	RegionResolver->Initialize(RegionBridge, AssetManager);

	RegionPresenter = NewObject<UVRegionPresenter>(this);
	RegionPresenter->Initialize(
		MeshPresenter,
		MaterialPresenter,
		LightPresenter,
		DecalPresenter,
		CollisionPresenter);

	// Start streaming on the local client.
	bSpatialStreamActive = true;
	bSpatialHasMore = true;
	bSpatialRequestInFlight = false;

	if (APawn* P = CachedPawn.Get())
	{
		SpatialOrigin = P->GetActorLocation();
	}
}

void AVRegionClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;

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
	APawn* Pawn = CachedPawn.Get();
	if (!Pawn)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			Pawn = PC->GetPawn();
			CachedPawn = Pawn;
		}
	}
	if (Pawn)
	{
		CurrentPos = Pawn->GetActorLocation();
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

	if (RegionResolver)
	{
		for (const TArray<FVMSpatialItemNet>& Batch : PendingSpatialBatches)
		{
			RegionResolver->OnSpatialBatchReceived(Batch);
		}
		PendingSpatialBatches.Reset();

		for (const TArray<FVMMaterial>& Batch : PendingMaterialBatches)
		{
			RegionResolver->OnMaterialsBatchReceived(Batch);
		}
		PendingMaterialBatches.Reset();

		for (const TArray<FVMCollision>& Batch : PendingCollisionBatches)
		{
			RegionResolver->OnCollisionsBatchReceived(Batch);
		}
		PendingCollisionBatches.Reset();
	}

	if (RegionResolver && RegionPresenter)
	{
		TArray<FResolvedItemBundle> Bundles;
		RegionResolver->GetResolvedBundles(MaxResolvedBundlesPerTick, Bundles);
		RegionPresenter->Commit(Bundles);
	}

	// If a request is in-flight, wait for the response before doing more.
	if (bSpatialRequestInFlight)
	{
		return;
	}

	// If the server told us there is no more data for the current stream, stop unless we recenter.
	if (!bSpatialHasMore)
	{
		return;
	}

	RequestNextSpatialBatch();
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

	PendingSpatialBatches.Add(Items);
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

void AVRegionClient::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	PendingMaterialBatches.Add(Materials);
}

void AVRegionClient::OnCollisionsBatchReceived(const TArray<FVMCollision>& Collisions)
{
	PendingCollisionBatches.Add(Collisions);
}

void AVRegionClient::OnSpatialItemRemoved(const FGuid& ItemId) const
{
	if (RegionResolver)
	{
		RegionResolver->OnSpatialItemRemoved(ItemId);
	}
	if (RegionPresenter)
	{
		RegionPresenter->ReleaseItem(ItemId);
	}
}
