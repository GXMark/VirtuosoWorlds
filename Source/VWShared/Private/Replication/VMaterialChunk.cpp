#include "Replication/VMaterialChunk.h"

#include "Net/UnrealNetwork.h"

// ----------------------
// FastArray callback impl
// ----------------------

void FVReplicatedMaterialItem::PreReplicatedRemove(const FVReplicatedMaterialItemArray& InArray)
{
	if (InArray.Owner)
	{
		InArray.Owner->HandleMaterialRemoved(Material.id.Value);
	}
}

void FVReplicatedMaterialItem::PostReplicatedAdd(const FVReplicatedMaterialItemArray& InArray)
{
	if (InArray.Owner)
	{
		InArray.Owner->HandleMaterialAdded(Material);
	}
}

void FVReplicatedMaterialItem::PostReplicatedChange(const FVReplicatedMaterialItemArray& InArray)
{
	if (InArray.Owner)
	{
		InArray.Owner->HandleMaterialChanged(Material);
	}
}

// ----------------------
// AVMaterialChunk
// ----------------------

AVMaterialChunk::AVMaterialChunk()
{
	bReplicates = true;
	SetReplicatingMovement(false);

	// Phase 1: correctness over interest filtering
	bAlwaysRelevant = true;

	MaterialItems.SetOwner(this);
}

void AVMaterialChunk::BeginPlay()
{
	Super::BeginPlay();
	MaterialItems.SetOwner(this);
}

void AVMaterialChunk::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVMaterialChunk, MaterialItems);
}

int32 AVMaterialChunk::FindIndexByMaterialID(const FGuid& InMaterialID) const
{
	for (int32 i = 0; i < MaterialItems.Items.Num(); ++i)
	{
		if (MaterialItems.Items[i].Material.id.Value == InMaterialID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

void AVMaterialChunk::ServerAddOrUpdateMaterial(const FVMMaterialNet& InMaterial)
{
	if (!HasAuthority())
	{
		return;
	}

	const FGuid MatID = InMaterial.id.Value;
	const int32 Index = FindIndexByMaterialID(MatID);

	if (Index != INDEX_NONE)
	{
		MaterialItems.Items[Index].Material = InMaterial;
		MaterialItems.MarkItemDirty(MaterialItems.Items[Index]);
	}
	else
	{
		FVReplicatedMaterialItem NewEntry;
		NewEntry.Material = InMaterial;

		MaterialItems.Items.Add(NewEntry);
		MaterialItems.MarkItemDirty(MaterialItems.Items.Last());
		MaterialItems.MarkArrayDirty();
	}
}

void AVMaterialChunk::ServerRemoveMaterial(const FGuid& InMaterialID)
{
	if (!HasAuthority())
	{
		return;
	}

	const int32 Index = FindIndexByMaterialID(InMaterialID);
	if (Index != INDEX_NONE)
	{
		MaterialItems.Items.RemoveAtSwap(Index);
		MaterialItems.MarkArrayDirty();
	}
}

void AVMaterialChunk::HandleMaterialAdded(const FVMMaterialNet& InMaterial)
{
	UE_LOG(LogTemp, Log, TEXT("[MatChunk] ADD Material=%s Name=%s"),
		*InMaterial.id.Value.ToString(), *InMaterial.name.ToString());
}

void AVMaterialChunk::HandleMaterialChanged(const FVMMaterialNet& InMaterial)
{
	UE_LOG(LogTemp, Verbose, TEXT("[MatChunk] CHG Material=%s Name=%s"),
		*InMaterial.id.Value.ToString(), *InMaterial.name.ToString());
}

void AVMaterialChunk::HandleMaterialRemoved(const FGuid& InMaterialID)
{
	UE_LOG(LogTemp, Log, TEXT("[MatChunk] REM Material=%s"), *InMaterialID.ToString());
}
