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

	bool MaterialIdMatches(const FGuid& MaterialGuid, uint32 MaterialId)
	{
		if (!MaterialGuid.IsValid() || MaterialId == 0)
		{
			return false;
		}

		return ResolveMaterialGuid(MaterialId) == MaterialGuid;
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
			++State.Generation;
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
			++State.Generation;
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
			PendingDestroyItemIds.Add(Job.ItemId);
			DirtyItemIds.Remove(Job.ItemId);
			break;
		}
		case EVRegionClientJobType::MaterialsBatchReceived:
		{
			if (AssetManager.IsValid())
			{
				AssetManager->SubmitMaterialItems(Job.MaterialsBatch);
			}
			if (Job.MaterialsBatch.IsEmpty())
			{
				break;
			}

			TSet<FGuid> UpdatedMaterials;
			UpdatedMaterials.Reserve(Job.MaterialsBatch.Num());
			for (const FVMMaterial& MaterialItem : Job.MaterialsBatch)
			{
				if (MaterialItem.id.IsValid())
				{
					UpdatedMaterials.Add(MaterialItem.id);
				}
			}

			if (UpdatedMaterials.IsEmpty())
			{
				break;
			}

			for (auto& ItemPair : ItemStates)
			{
				const FGuid& ItemId = ItemPair.Key;
				FRegionClientItemState& State = ItemPair.Value;
				bool bUsesUpdatedMaterial = false;
				for (const uint32 MaterialId : State.DesiredMaterialIdsBySlot)
				{
					const FGuid MaterialGuid = ResolveMaterialGuid(MaterialId);
					if (MaterialGuid.IsValid() && UpdatedMaterials.Contains(MaterialGuid))
					{
						bUsesUpdatedMaterial = true;
						break;
					}
				}

				if (bUsesUpdatedMaterial)
				{
					State.AppliedMask &= ~kAppliedMaterialsMask;
					State.LegacyFallbackMask &= ~kFallbackMaterialsMask;
					DirtyItemIds.Add(ItemId);
				}
			}
			break;
		}
		default:
			break;
	}
}

void UVRegionClientResolver::EmitRenderWork(FVRegionRenderQueue& RenderQueue, UVAssetManager* InAssetManager)
{
	if (!PendingDestroyItemIds.IsEmpty())
	{
		for (const FGuid& ItemId : PendingDestroyItemIds)
		{
			if (FRegionClientItemState* State = ItemStates.Find(ItemId))
			{
				FVRegionRenderWorkItem WorkItem;
				WorkItem.WorkType = EVRegionRenderWorkType::Destroy;
				WorkItem.ItemId = ItemId;
				WorkItem.Generation = State->Generation;
				RenderQueue.Enqueue(MoveTemp(WorkItem));
			}
		}
		PendingDestroyItemIds.Reset();
	}

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
			WorkItem.Generation = State->Generation;
			RenderQueue.Enqueue(MoveTemp(WorkItem));
		}

		if (bMaterialsRequired && !bMaterialsApplied && bMeshApplied && bMaterialsReady)
		{
			FVRegionRenderWorkItem WorkItem;
			WorkItem.WorkType = EVRegionRenderWorkType::ApplyMaterials;
			WorkItem.ItemId = ItemId;
			WorkItem.MaterialIdsBySlot = State->DesiredMaterialIdsBySlot;
			WorkItem.Generation = State->Generation;
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

bool UVRegionClientResolver::IsItemGenerationCurrent(const FGuid& ItemId, uint32 ExpectedGeneration) const
{
	if (const FRegionClientItemState* State = ItemStates.Find(ItemId))
	{
		return State->Generation == ExpectedGeneration;
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

bool UVRegionClientResolver::FinalizeDestroy(const FGuid& ItemId, uint32 ExpectedGeneration)
{
	const FRegionClientItemState* State = ItemStates.Find(ItemId);
	if (!State || State->Generation != ExpectedGeneration)
	{
		return false;
	}

	ItemStates.Remove(ItemId);
	DirtyItemIds.Remove(ItemId);
	PendingDestroyItemIds.Remove(ItemId);
	return true;
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

bool UVRegionClientResolver::FindItemUsingMaterial(const FGuid& MaterialId, FGuid& OutItemId, uint32& OutGeneration) const
{
	if (!MaterialId.IsValid())
	{
		return false;
	}

	for (const auto& ItemPair : ItemStates)
	{
		const FRegionClientItemState& State = ItemPair.Value;
		for (const uint32 MaterialSlotId : State.DesiredMaterialIdsBySlot)
		{
			if (MaterialIdMatches(MaterialId, MaterialSlotId))
			{
				OutItemId = ItemPair.Key;
				OutGeneration = State.Generation;
				return true;
			}
		}
	}

	return false;
}

bool UVRegionClientResolver::IsItemUsingMaterial(const FGuid& ItemId, const FGuid& MaterialId, uint32 ExpectedGeneration) const
{
	const FRegionClientItemState* State = ItemStates.Find(ItemId);
	if (!State)
	{
		return false;
	}

	if (State->Generation != ExpectedGeneration)
	{
		return false;
	}

	for (const uint32 MaterialSlotId : State->DesiredMaterialIdsBySlot)
	{
		if (MaterialIdMatches(MaterialId, MaterialSlotId))
		{
			return true;
		}
	}

	return false;
}
