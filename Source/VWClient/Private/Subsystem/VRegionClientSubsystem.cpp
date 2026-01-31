#include "Subsystem/VRegionClientSubsystem.h"

#include "Interface/VSpatialItemActorInterface.h"
#include "Presentation/VMaterialPresenterApplier.h"
#include "Presentation/VMaterialResolver.h"
#include "Region/VRegionClientBridge.h"
#include "Subsystem/VAssetManager.h"
#include "Utility/VSpatialActorEvents.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void URegionClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() != NM_Client)
	{
		return;
	}

	AssetManager = World->GetSubsystem<UVAssetManager>();

	if (APlayerController* PC = World->GetFirstPlayerController())
	{
		RegionBridge = NewObject<URegionClientBridge>(this);
		RegionBridge->Initialize(PC);
	}

	MaterialResolver = NewObject<UVMaterialResolver>(this);
	MaterialResolver->Initialize(AssetManager, RegionBridge);

	MaterialPresenter = NewObject<UVMaterialPresenterApplier>(this);
	MaterialPresenter->Initialize(MaterialResolver);

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
	if (MaterialResolver)
	{
		MaterialResolver->OnMaterialsBatchReceived(Materials);
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
	State.bMaterialsPending = true;

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
			if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
			{
				if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
				{
					MaterialPresenter->ForgetMeshComponent(MeshComp);
				}
			}
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
		State.bMeshPending = true;
		State.bMaterialsPending = true;

		TryApplyMesh(ItemId, Actor, State);
		TryApplyMaterials(ItemId, Actor, State);
	}
}

void URegionClientSubsystem::TryApplyMesh(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State) const
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
					TryApplyMaterials(ItemId, Actor, State);
				}
			}
		}
	}
}

void URegionClientSubsystem::TryApplyMaterials(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State) const
{
	if (!Actor || !MaterialPresenter)
	{
		return;
	}

	if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
	{
		if (UStaticMeshComponent* MeshComp = MeshInterface->GetSpatialMeshComponent())
		{
			MaterialPresenter->ApplyMaterials(MeshComp, State.MaterialIdsBySlot);
			State.bMaterialsPending = false;
		}
	}
}
