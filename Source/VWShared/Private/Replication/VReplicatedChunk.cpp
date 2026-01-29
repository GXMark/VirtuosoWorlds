#include "Replication/VReplicatedChunk.h"

#include "Net/UnrealNetwork.h"

// ----------------------
// FastArray callback impl
// ----------------------

void FVReplicatedSpatialItem::PreReplicatedRemove(const FVReplicatedSpatialItemArray& InArray) const
{
	if (InArray.Owner)
	{
		InArray.Owner->HandleSpatialItemRemoved(Item.ItemID);
	}
}

void FVReplicatedSpatialItem::PostReplicatedAdd(const FVReplicatedSpatialItemArray& InArray) const
{
	if (InArray.Owner)
	{
		InArray.Owner->HandleSpatialItemAdded(Item);
	}
}

void FVReplicatedSpatialItem::PostReplicatedChange(const FVReplicatedSpatialItemArray& InArray) const
{
	if (InArray.Owner)
	{
		InArray.Owner->HandleSpatialItemChanged(Item);
	}
}

// ----------------------
// AVReplicatedChunk
// ----------------------

AVReplicatedChunk::AVReplicatedChunk()
{
	bReplicates = true;
	SetReplicatingMovement(false);

	// Phase 1: correctness over relevancy filtering
	bAlwaysRelevant = true;

	SpatialItems.SetOwner(this);
}

void AVReplicatedChunk::BeginPlay()
{
	Super::BeginPlay();
	SpatialItems.SetOwner(this);
}

void AVReplicatedChunk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVReplicatedChunk, ChunkID);
	DOREPLIFETIME(AVReplicatedChunk, SpatialItems);
}

int32 AVReplicatedChunk::FindIndexByItemID(const FVMGuidNet& InItemID) const
{
	for (int32 i = 0; i < SpatialItems.Items.Num(); ++i)
	{
		if (SpatialItems.Items[i].Item.ItemID.Identical(&InItemID, 0))
		{
			return i;
		}
	}
	return INDEX_NONE;
}

void AVReplicatedChunk::ServerAddOrUpdateSpatialItem(const FVMSpatialItemNet& InItem)
{
	if (!HasAuthority())
	{
		return;
	}

	const int32 Index = FindIndexByItemID(InItem.ItemID);
	const bool bWasUpdate = (Index != INDEX_NONE);

	if (bWasUpdate)
	{
		SpatialItems.Items[Index].Item = InItem;
		SpatialItems.MarkItemDirty(SpatialItems.Items[Index]);
	}
	else
	{
		FVReplicatedSpatialItem NewEntry;
		NewEntry.Item = InItem;
		SpatialItems.Items.Add(NewEntry);
		SpatialItems.MarkItemDirty(SpatialItems.Items.Last());
		SpatialItems.MarkArrayDirty();
	}

	// Optional: allow listen-server local presentation without waiting for replication loop.
	if (GetNetMode() != NM_DedicatedServer)
	{
		if (bWasUpdate)
		{
			HandleSpatialItemChanged(InItem);
		}
		else
		{
			HandleSpatialItemAdded(InItem);
		}
	}
}

void AVReplicatedChunk::ServerRemoveSpatialItem(const FVMGuidNet& InItemID)
{
	if (!HasAuthority())
	{
		return;
	}

	const int32 Index = FindIndexByItemID(InItemID);
	if (Index != INDEX_NONE)
	{
		SpatialItems.Items.RemoveAtSwap(Index);
		SpatialItems.MarkArrayDirty();

		// Optional: allow listen-server local presentation.
		if (GetNetMode() != NM_DedicatedServer)
		{
			HandleSpatialItemRemoved(InItemID);
		}
	}
}

// ----------------------
// Data-only FastArray hooks
// ----------------------

void AVReplicatedChunk::HandleSpatialItemAdded(const FVMSpatialItemNet& InItem) const
{
	OnItemAdded.Broadcast(InItem);
}

void AVReplicatedChunk::HandleSpatialItemChanged(const FVMSpatialItemNet& InItem) const
{
	OnItemUpdated.Broadcast(InItem);
}

void AVReplicatedChunk::HandleSpatialItemRemoved(const FVMGuidNet& InItemID) const
{
	OnItemRemoved.Broadcast(InItemID.Value);
}
