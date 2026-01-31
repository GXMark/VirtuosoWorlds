#include "Subsystem/VRegionClientSubsystem.h"

#include "Interface/VSpatialItemActorInterface.h"
#include "Presentation/VMaterialPresenter.h"
#include "Region/VRegionClientBridge.h"
#include "Subsystem/VAssetManager.h"
#include "Utility/VSpatialActorEvents.h"
#include "Algo/Unique.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInterface.h"

void URegionClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() != NM_Client)
	{
		return;
	}

	AssetManager = World->GetSubsystem<UVAssetManager>();

	MaterialPresenter = NewObject<UVMaterialPresenter>(this);
	MaterialPresenter->Initialize();

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		RegionBridge = NewObject<URegionClientBridge>(this);
		RegionBridge->Initialize(PC);
	}

	PostInitHandle = FSpatialActorEvents::OnSpatialActorPostInit().AddUObject(
		this, &URegionClientSubsystem::HandleSpatialActorPostInit);
	MeshAssetHandle = FSpatialActorEvents::OnSpatialActorMeshAssetIdChanged().AddUObject(
		this, &URegionClientSubsystem::HandleMeshAssetIdChanged);
	MaterialIdsHandle = FSpatialActorEvents::OnSpatialActorMaterialIdsChanged().AddUObject(
		this, &URegionClientSubsystem::HandleMaterialIdsChanged);
}

void URegionClientSubsystem::Deinitialize()
{
	if (PostInitHandle.IsValid())
	{
		FSpatialActorEvents::OnSpatialActorPostInit().Remove(PostInitHandle);
	}
	if (MeshAssetHandle.IsValid())
	{
		FSpatialActorEvents::OnSpatialActorMeshAssetIdChanged().Remove(MeshAssetHandle);
	}
	if (MaterialIdsHandle.IsValid())
	{
		FSpatialActorEvents::OnSpatialActorMaterialIdsChanged().Remove(MaterialIdsHandle);
	}

	Super::Deinitialize();
}

void URegionClientSubsystem::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	if (AssetManager)
	{
		AssetManager->SubmitMaterialItems(Materials);
	}
}

void URegionClientSubsystem::HandleSpatialActorPostInit(AActor* Actor, const FSpatialItemId& SpatialId)
{
	if (!Actor || !GetWorld() || GetWorld()->GetNetMode() != NM_Client)
	{
		return;
	}

	const FGuid ItemId = ResolveSpatialId(Actor, &SpatialId);
	if (!ItemId.IsValid())
	{
		return;
	}

	RegisterActor(Actor, ItemId);
	SyncVisualStateFromActor(Actor, ItemId);
}

void URegionClientSubsystem::HandleMeshAssetIdChanged(AActor* Actor, const FMeshAssetId& MeshAssetId)
{
	if (!Actor)
	{
		return;
	}

	const FGuid ItemId = ResolveSpatialId(Actor);
	if (!ItemId.IsValid())
	{
		return;
	}

	FSpatialActorVisualState& State = VisualStates.FindOrAdd(ItemId);
	State.MeshAssetId = MeshAssetId;
	State.bMeshPending = true;

	TryApplyMesh(ItemId, Actor, State);
}

void URegionClientSubsystem::HandleMaterialIdsChanged(AActor* Actor, const TArray<uint32>& MaterialIds)
{
	if (!Actor)
	{
		return;
	}

	const FGuid ItemId = ResolveSpatialId(Actor);
	if (!ItemId.IsValid())
	{
		return;
	}

	FSpatialActorVisualState& State = VisualStates.FindOrAdd(ItemId);
	State.MaterialIdsBySlot = MaterialIds;
	State.MaterialRevision += 1;
	State.PendingMaterialRequests = 0;
	State.ResolvedMaterials.SetNum(MaterialIds.Num());
	State.bMaterialsPending = true;

	RequestMaterialUpdates(ItemId, State);
	TryApplyMaterials(ItemId, Actor, State);
}

void URegionClientSubsystem::HandleActorDestroyed(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	const TWeakObjectPtr<AActor> ActorKey(Actor);
	if (const FGuid* ItemId = ActorToSpatialId.Find(ActorKey))
	{
		SpatialItemActors.Remove(*ItemId);
		VisualStates.Remove(*ItemId);
		ActorToSpatialId.Remove(ActorKey);

		if (MaterialPresenter)
		{
			MaterialPresenter->ForgetItem(*ItemId);
		}
	}
}

void URegionClientSubsystem::RegisterActor(AActor* Actor, const FGuid& ItemId)
{
	if (!Actor)
	{
		return;
	}

	const TWeakObjectPtr<AActor> ActorKey(Actor);
	SpatialItemActors.FindOrAdd(ItemId) = Actor;
	ActorToSpatialId.FindOrAdd(ActorKey) = ItemId;

	Actor->OnDestroyed.AddUniqueDynamic(this, &URegionClientSubsystem::HandleActorDestroyed);
}

FGuid URegionClientSubsystem::ResolveSpatialId(AActor* Actor, const FSpatialItemId* SpatialIdOverride)
{
	if (!Actor)
	{
		return FGuid();
	}

	if (SpatialIdOverride)
	{
		const FGuid OverrideId = SpatialIdOverride->Value.Value;
		if (OverrideId.IsValid())
		{
			return OverrideId;
		}
	}

	const TWeakObjectPtr<AActor> ActorKey(Actor);
	if (const FGuid* ItemId = ActorToSpatialId.Find(ActorKey))
	{
		return *ItemId;
	}

	if (ISpatialItemActorInterface* SpatialInterface = Cast<ISpatialItemActorInterface>(Actor))
	{
		const FGuid InterfaceId = SpatialInterface->GetSpatialItemId().Value.Value;
		if (InterfaceId.IsValid())
		{
			RegisterActor(Actor, InterfaceId);
			return InterfaceId;
		}
	}

	return FGuid();
}

void URegionClientSubsystem::SyncVisualStateFromActor(AActor* Actor, const FGuid& ItemId)
{
	if (!Actor)
	{
		return;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		FSpatialActorVisualState& State = VisualStates.FindOrAdd(ItemId);
		State.MeshAssetId = MeshInterface->GetSpatialMeshAssetId();
		State.MaterialIdsBySlot = MeshInterface->GetSpatialMaterialIdsBySlot();
		State.MaterialRevision += 1;
		State.ResolvedMaterials.SetNum(State.MaterialIdsBySlot.Num());
		State.bMeshPending = true;
		State.bMaterialsPending = true;

		RequestMaterialUpdates(ItemId, State);
		TryApplyMesh(ItemId, Actor, State);
		TryApplyMaterials(ItemId, Actor, State);
	}
}

void URegionClientSubsystem::RequestMaterialUpdates(const FGuid& ItemId, FSpatialActorVisualState& State)
{
	if (!AssetManager)
	{
		return;
	}

	TArray<FGuid> RequestedMaterialIds;
	RequestedMaterialIds.Reserve(State.MaterialIdsBySlot.Num());

	const int32 Revision = State.MaterialRevision;
	State.PendingMaterialRequests = 0;

	for (int32 SlotIndex = 0; SlotIndex < State.MaterialIdsBySlot.Num(); ++SlotIndex)
	{
		const uint32 MaterialId = State.MaterialIdsBySlot[SlotIndex];
		const FGuid MaterialGuid = ResolveMaterialGuid(MaterialId);
		if (!MaterialGuid.IsValid())
		{
			continue;
		}

		RequestedMaterialIds.Add(MaterialGuid);
		State.PendingMaterialRequests += 1;

		AssetManager->RequestMaterialInstanceAsync(
			MaterialGuid,
			FVOnMaterialInstanceLoaded::CreateUObject(
				this,
				&URegionClientSubsystem::OnMaterialResolved,
				ItemId,
				Revision,
				SlotIndex));
	}

	if (RegionBridge && RequestedMaterialIds.Num() > 0)
	{
		RequestedMaterialIds.Sort();
		RequestedMaterialIds.SetNum(Algo::Unique(RequestedMaterialIds));
		RegionBridge->RequestMaterialsBatch(RequestedMaterialIds);
	}
}

void URegionClientSubsystem::TryApplyMesh(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State)
{
	if (!Actor || !AssetManager)
	{
		return;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		const FGuid MeshId = State.MeshAssetId.Value.Value;
		if (!MeshId.IsValid())
		{
			return;
		}

		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			if (AssetManager->MeshAgent)
			{
				if (UStaticMesh* MeshAsset = AssetManager->MeshAgent->GetMesh(MeshId))
				{
					MeshComp->SetStaticMesh(MeshAsset);
					State.bMeshPending = false;
				}
			}
		}
	}
}

void URegionClientSubsystem::TryApplyMaterials(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State)
{
	if (!Actor || State.PendingMaterialRequests > 0 || !MaterialPresenter)
	{
		return;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			TArray<UMaterialInterface*> Materials;
			Materials.Reserve(State.ResolvedMaterials.Num());
			for (const TObjectPtr<UMaterialInterface>& Material : State.ResolvedMaterials)
			{
				Materials.Add(Material.Get());
			}
			MaterialPresenter->ApplyMaterials(ItemId, MeshComp, Materials);
			State.bMaterialsPending = false;
		}
	}
}

void URegionClientSubsystem::OnMaterialResolved(
	const FGuid ItemId,
	int32 Revision,
	int32 SlotIndex,
	UMaterialInterface* Material)
{
	FSpatialActorVisualState* State = VisualStates.Find(ItemId);
	if (!State || State->MaterialRevision != Revision)
	{
		return;
	}

	if (!State->ResolvedMaterials.IsValidIndex(SlotIndex))
	{
		return;
	}

	State->ResolvedMaterials[SlotIndex] = Material;
	State->PendingMaterialRequests = FMath::Max(0, State->PendingMaterialRequests - 1);

	if (TWeakObjectPtr<AActor>* Actor = SpatialItemActors.Find(ItemId))
	{
		TryApplyMaterials(ItemId, Actor->Get(), *State);
	}
}

FGuid URegionClientSubsystem::ResolveMaterialGuid(uint32 MaterialId) const
{
	if (MaterialId == 0)
	{
		return FGuid();
	}

	return FGuid(static_cast<int32>(MaterialId), 0, 0, 0);
}
