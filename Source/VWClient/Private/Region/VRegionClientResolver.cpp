#include "Region/VRegionClientResolver.h"

#include "Subsystem/VAssetManager.h"

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
			++State.Generation;
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
