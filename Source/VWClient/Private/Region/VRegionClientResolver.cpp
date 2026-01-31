#include "Region/VRegionClientResolver.h"

#include "Interface/VSpatialItemActorInterface.h"
#include "Model/Constant/VConstants.h"
#include "Subsystem/VAssetManager.h"

namespace
{
	constexpr uint8 kAppliedMeshMask = 1 << 0;
	constexpr uint8 kAppliedMaterialsMask = 1 << 1;
	constexpr uint8 kFallbackMeshMask = 1 << 0;
	constexpr uint8 kFallbackMaterialsMask = 1 << 1;

	FGuid ResolveMaterialGuid(uint32 MaterialId)
	{
		return MaterialId == 0 ? FGuid() : FGuid(static_cast<int32>(MaterialId), 0, 0, 0);
	}

	bool AreMaterialsReady(const TArray<uint32>& MaterialIdsBySlot, UVAssetManager* AssetManager)
	{
		if (!AssetManager)
		{
			return false;
		}

		for (const uint32 MaterialId : MaterialIdsBySlot)
		{
			if (MaterialId == 0)
			{
				continue;
			}

			const FGuid MaterialGuid = ResolveMaterialGuid(MaterialId);
			if (!MaterialGuid.IsValid() || !AssetManager->IsMemoryCached(MaterialGuid, FVAssetType::Material))
			{
				return false;
			}
		}

		return true;
	}
} // namespace

void UVRegionClientResolver::Initialize(UVAssetManager* InAssetManager)
{
	AssetManager = InAssetManager;
}

void UVRegionClientResolver::ConsumeJob(const FVRegionClientJob& Job)
{
	switch (Job.JobType)
	{
		case EVRegionClientJobType::SpatialActorObserved:
		{
			if (!Job.ItemId.IsValid())
			{
				return;
			}

			FRegionClientItemState& State = ItemStates.FindOrAdd(Job.ItemId);
			State.Actor = Job.Actor;
			if (AActor* Actor = Job.Actor.Get())
			{
				if (ISpatialMeshItemActorInterface* MeshInterface = Cast<ISpatialMeshItemActorInterface>(Actor))
				{
					State.DesiredMeshAssetId = MeshInterface->GetSpatialMeshAssetId();
					State.DesiredMaterialIdsBySlot = MeshInterface->GetSpatialMaterialIdsBySlot();
				}
			}
			State.AppliedMask = 0;
			State.LegacyFallbackMask = 0;
			DirtyItemIds.Add(Job.ItemId);
			break;
		}
		case EVRegionClientJobType::MeshAssetIdChanged:
		{
			if (!Job.ItemId.IsValid())
			{
				return;
			}

			FRegionClientItemState& State = ItemStates.FindOrAdd(Job.ItemId);
			State.DesiredMeshAssetId = Job.MeshAssetId;
			State.AppliedMask &= ~kAppliedMeshMask;
			State.AppliedMask &= ~kAppliedMaterialsMask;
			State.LegacyFallbackMask &= ~kFallbackMeshMask;
			State.LegacyFallbackMask &= ~kFallbackMaterialsMask;
			DirtyItemIds.Add(Job.ItemId);
			break;
		}
		case EVRegionClientJobType::MaterialIdsChanged:
		{
			if (!Job.ItemId.IsValid())
			{
				return;
			}

			FRegionClientItemState& State = ItemStates.FindOrAdd(Job.ItemId);
			State.DesiredMaterialIdsBySlot = Job.MaterialIdsBySlot;
			State.AppliedMask &= ~kAppliedMaterialsMask;
			State.LegacyFallbackMask &= ~kFallbackMaterialsMask;
			DirtyItemIds.Add(Job.ItemId);
			break;
		}
		case EVRegionClientJobType::ActorDestroyed:
		{
			if (!Job.ItemId.IsValid())
			{
				return;
			}

			FRegionClientItemState& State = ItemStates.FindOrAdd(Job.ItemId);
			State.Actor.Reset();
			State.DesiredMeshAssetId = FMeshAssetId();
			State.DesiredMaterialIdsBySlot.Reset();
			State.AppliedMask = 0;
			State.LegacyFallbackMask = 0;
			++State.Generation;
			DirtyItemIds.Remove(Job.ItemId);
			break;
		}
		case EVRegionClientJobType::MaterialsBatchReceived:
		{
			if (AssetManager.IsValid())
			{
				AssetManager->SubmitMaterialItems(Job.MaterialsBatch);
			}
			break;
		}
		default:
			break;
	}
}

void UVRegionClientResolver::EmitRenderWork(FVRegionRenderQueue& RenderQueue, UVAssetManager* InAssetManager)
{
	UVAssetManager* EffectiveAssetManager = InAssetManager;
	if (!EffectiveAssetManager && AssetManager.IsValid())
	{
		EffectiveAssetManager = AssetManager.Get();
	}

	if (!EffectiveAssetManager || DirtyItemIds.IsEmpty())
	{
		return;
	}

	TSet<FGuid> NextDirty;
	for (const FGuid& ItemId : DirtyItemIds)
	{
		FRegionClientItemState* State = ItemStates.Find(ItemId);
		if (!State)
		{
			continue;
		}

		const FGuid MeshGuid = State->DesiredMeshAssetId.Value.Value;
		const bool bMeshRequired = MeshGuid.IsValid();
		const bool bMeshApplied = !bMeshRequired || (State->AppliedMask & kAppliedMeshMask);
		const bool bMeshReady = !bMeshRequired
			|| EffectiveAssetManager->IsMemoryCached(MeshGuid, FVAssetType::Mesh);

		const bool bMaterialsRequired = State->DesiredMaterialIdsBySlot.Num() > 0;
		const bool bMaterialsApplied = !bMaterialsRequired || (State->AppliedMask & kAppliedMaterialsMask);
		const bool bMaterialsReady = !bMaterialsRequired
			|| AreMaterialsReady(State->DesiredMaterialIdsBySlot, EffectiveAssetManager);

		if (bMeshRequired && !bMeshApplied && bMeshReady)
		{
			FVRegionRenderWorkItem WorkItem;
			WorkItem.WorkType = EVRegionRenderWorkType::SetMesh;
			WorkItem.ItemId = ItemId;
			WorkItem.MeshAssetId = State->DesiredMeshAssetId;
			RenderQueue.Enqueue(MoveTemp(WorkItem));
		}

		if (bMaterialsRequired && !bMaterialsApplied && bMeshApplied && bMaterialsReady)
		{
			FVRegionRenderWorkItem WorkItem;
			WorkItem.WorkType = EVRegionRenderWorkType::ApplyMaterials;
			WorkItem.ItemId = ItemId;
			WorkItem.MaterialIdsBySlot = State->DesiredMaterialIdsBySlot;
			RenderQueue.Enqueue(MoveTemp(WorkItem));
		}

		if ((bMeshRequired && !bMeshApplied) || (bMaterialsRequired && !bMaterialsApplied))
		{
			NextDirty.Add(ItemId);
		}
	}

	DirtyItemIds = MoveTemp(NextDirty);
}

bool UVRegionClientResolver::GetItemSnapshot(const FGuid& ItemId, FRegionClientItemSnapshot& OutSnapshot) const
{
	if (const FRegionClientItemState* State = ItemStates.Find(ItemId))
	{
		OutSnapshot.Actor = State->Actor;
		OutSnapshot.MeshAssetId = State->DesiredMeshAssetId;
		OutSnapshot.MaterialIdsBySlot = State->DesiredMaterialIdsBySlot;
		OutSnapshot.Generation = State->Generation;
		return true;
	}

	return false;
}

void UVRegionClientResolver::MarkApplied(const FGuid& ItemId, EVRegionRenderWorkType WorkType)
{
	FRegionClientItemState* State = ItemStates.Find(ItemId);
	if (!State)
	{
		return;
	}

	switch (WorkType)
	{
	case EVRegionRenderWorkType::SetMesh:
		State->AppliedMask |= kAppliedMeshMask;
		break;
	case EVRegionRenderWorkType::ApplyMaterials:
		State->AppliedMask |= kAppliedMaterialsMask;
		break;
	default:
		break;
	}
}

bool UVRegionClientResolver::MarkLegacyFallbackAttempted(const FGuid& ItemId, EVRegionRenderWorkType WorkType)
{
	FRegionClientItemState* State = ItemStates.Find(ItemId);
	if (!State)
	{
		return false;
	}

	uint8 Mask = 0;
	switch (WorkType)
	{
	case EVRegionRenderWorkType::SetMesh:
		Mask = kFallbackMeshMask;
		break;
	case EVRegionRenderWorkType::ApplyMaterials:
		Mask = kFallbackMaterialsMask;
		break;
	default:
		break;
	}

	if (Mask == 0 || (State->LegacyFallbackMask & Mask))
	{
		return false;
	}

	State->LegacyFallbackMask |= Mask;
	return true;
}
