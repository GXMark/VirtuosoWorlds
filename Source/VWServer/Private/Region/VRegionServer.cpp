#include "Region/VRegionServer.h"

#include "Replication/VReplicationManager.h"
#include "Replication/VSpatialItemActor.h"

AVRegionServer::AVRegionServer()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
}

void AVRegionServer::BeginPlay()
{
	Super::BeginPlay();
}

void AVRegionServer::LoadSpatialItems(const TArray<FVMSpatialItemNet>& Items)
{
	if (!HasAuthority())
	{
		return;
	}

	for (const FVMSpatialItemNet& Item : Items)
	{
		UpsertSpatialItem(Item);
	}
}

void AVRegionServer::UpsertSpatialItem(const FVMSpatialItemNet& Item)
{
	if (!HasAuthority())
	{
		return;
	}

	const FGuid ItemId = Item.ItemID.Value;
	if (!ItemId.IsValid())
	{
		return;
	}

	SpatialItemState.Add(ItemId, Item);
	UpdateItemCell(ItemId, FVector(Item.Transform.Location));

	if (TObjectPtr<AVSpatialItemActor>* ExistingActor = SpatialItemActors.Find(ItemId))
	{
		AVSpatialItemActor* Actor = ExistingActor->Get();
		if (IsValid(Actor))
		{
			Actor->UpdateSpatialItem(Item);
		}
		else
		{
			SpatialItemActors.Remove(ItemId);
		}
	}

	if (!SpatialItemActors.Contains(ItemId))
	{
		if (AVSpatialItemActor* NewActor = SpawnSpatialItemActor(Item))
		{
			SpatialItemActors.Add(ItemId, NewActor);
			RegisterSpatialItemActor(NewActor, ItemId);
		}
	}
}

void AVRegionServer::RemoveSpatialItem(const FGuid& ItemId)
{
	if (!HasAuthority() || !ItemId.IsValid())
	{
		return;
	}

	SpatialItemState.Remove(ItemId);
	RemoveItemFromCell(ItemId);

	if (TObjectPtr<AVSpatialItemActor>* ExistingActor = SpatialItemActors.Find(ItemId))
	{
		AVSpatialItemActor* Actor = ExistingActor->Get();
		if (IsValid(Actor))
		{
			if (UVReplicationManager* Manager = GetWorld()->GetSubsystem<UVReplicationManager>())
			{
				Manager->UnregisterSpatialItemActor(ItemId);
			}

			Actor->Destroy();
		}

		SpatialItemActors.Remove(ItemId);
	}
}

void AVRegionServer::UpdateConnectionRelevancy(UNetConnection* Connection, const TSet<FGuid>& RelevantItems)
{
	if (!HasAuthority())
	{
		return;
	}

	if (UVReplicationManager* Manager = GetWorld()->GetSubsystem<UVReplicationManager>())
	{
		Manager->SetConnectionRelevancy(Connection, RelevantItems);
	}
}

void AVRegionServer::UpdateConnectionLocation(UNetConnection* Connection, const FVector& Location)
{
	if (!HasAuthority() || !Connection)
	{
		return;
	}

	PruneInvalidConnections();

	const FIntPoint NewCell = GetCellCoord(Location);
	FConnectionRelevancyState& State = ConnectionStates.FindOrAdd(Connection);

	if (!State.bInitialized)
	{
		State.bInitialized = true;
		State.CurrentCell = NewCell;
		State.ActiveCells = BuildCellWindow(NewCell, OuterCellRadius);

		TSet<FGuid> RelevantItems;
		GatherItemsForCells(State.ActiveCells, RelevantItems);

		if (UVReplicationManager* Manager = GetWorld()->GetSubsystem<UVReplicationManager>())
		{
			Manager->SetConnectionRelevancy(Connection, RelevantItems);
		}
		return;
	}

	const int32 DeltaX = FMath::Abs(NewCell.X - State.CurrentCell.X);
	const int32 DeltaY = FMath::Abs(NewCell.Y - State.CurrentCell.Y);
	const bool bWithinInner = DeltaX <= InnerCellRadius && DeltaY <= InnerCellRadius;
	if (bWithinInner)
	{
		return;
	}

	State.CurrentCell = NewCell;
	const TSet<FIntPoint> NewActiveCells = BuildCellWindow(NewCell, OuterCellRadius);

	TSet<FIntPoint> EnteredCells;
	EnteredCells.Reserve(NewActiveCells.Num());
	for (const FIntPoint& Cell : NewActiveCells)
	{
		if (!State.ActiveCells.Contains(Cell))
		{
			EnteredCells.Add(Cell);
		}
	}

	TSet<FIntPoint> ExitedCells;
	ExitedCells.Reserve(State.ActiveCells.Num());
	for (const FIntPoint& Cell : State.ActiveCells)
	{
		if (!NewActiveCells.Contains(Cell))
		{
			ExitedCells.Add(Cell);
		}
	}

	State.ActiveCells = NewActiveCells;

	TSet<FGuid> EnteredItems;
	TSet<FGuid> ExitedItems;
	GatherItemsForCells(EnteredCells, EnteredItems);
	GatherItemsForCells(ExitedCells, ExitedItems);

	if (UVReplicationManager* Manager = GetWorld()->GetSubsystem<UVReplicationManager>())
	{
		if (EnteredItems.Num() > 0)
		{
			Manager->AddRelevant(Connection, EnteredItems);
		}

		if (ExitedItems.Num() > 0)
		{
			Manager->RemoveRelevant(Connection, ExitedItems);
		}
	}
}

AVSpatialItemActor* AVRegionServer::SpawnSpatialItemActor(const FVMSpatialItemNet& Item)
{
	if (!HasAuthority())
	{
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;

	AVSpatialItemActor* Actor = World->SpawnActor<AVSpatialItemActor>(AVSpatialItemActor::StaticClass(), FTransform::Identity, Params);
	if (Actor)
	{
		Actor->InitializeSpatialItem(Item);
	}

	return Actor;
}

void AVRegionServer::RegisterSpatialItemActor(AVSpatialItemActor* Actor, const FGuid& ItemId)
{
	if (!Actor || !HasAuthority())
	{
		return;
	}

	if (UVReplicationManager* Manager = GetWorld()->GetSubsystem<UVReplicationManager>())
	{
		Manager->RegisterSpatialItemActor(Actor, ItemId);
	}
}

FIntPoint AVRegionServer::GetCellCoord(const FVector& Location) const
{
	const float SafeCellSize = FMath::Max(CellSize, 1.f);
	return FIntPoint(
		FMath::FloorToInt(Location.X / SafeCellSize),
		FMath::FloorToInt(Location.Y / SafeCellSize));
}

void AVRegionServer::UpdateItemCell(const FGuid& ItemId, const FVector& Location)
{
	if (!ItemId.IsValid())
	{
		return;
	}

	const FIntPoint NewCell = GetCellCoord(Location);
	if (const FIntPoint* ExistingCell = ItemCells.Find(ItemId))
	{
		if (*ExistingCell == NewCell)
		{
			return;
		}

		if (TSet<FGuid>* CellSet = CellBuckets.Find(*ExistingCell))
		{
			CellSet->Remove(ItemId);
			if (CellSet->Num() == 0)
			{
				CellBuckets.Remove(*ExistingCell);
			}
		}
	}

	ItemCells.Add(ItemId, NewCell);
	CellBuckets.FindOrAdd(NewCell).Add(ItemId);
}

void AVRegionServer::RemoveItemFromCell(const FGuid& ItemId)
{
	if (!ItemId.IsValid())
	{
		return;
	}

	if (const FIntPoint* ExistingCell = ItemCells.Find(ItemId))
	{
		if (TSet<FGuid>* CellSet = CellBuckets.Find(*ExistingCell))
		{
			CellSet->Remove(ItemId);
			if (CellSet->Num() == 0)
			{
				CellBuckets.Remove(*ExistingCell);
			}
		}

		ItemCells.Remove(ItemId);
	}
}

TSet<FIntPoint> AVRegionServer::BuildCellWindow(const FIntPoint& Center, int32 Radius) const
{
	TSet<FIntPoint> Cells;
	if (Radius < 0)
	{
		return Cells;
	}

	const int32 SafeRadius = FMath::Max(Radius, 0);
	for (int32 Dx = -SafeRadius; Dx <= SafeRadius; ++Dx)
	{
		for (int32 Dy = -SafeRadius; Dy <= SafeRadius; ++Dy)
		{
			Cells.Add(FIntPoint(Center.X + Dx, Center.Y + Dy));
		}
	}
	return Cells;
}

void AVRegionServer::GatherItemsForCells(const TSet<FIntPoint>& Cells, TSet<FGuid>& OutItems) const
{
	for (const FIntPoint& Cell : Cells)
	{
		if (const TSet<FGuid>* Items = CellBuckets.Find(Cell))
		{
			OutItems.Append(*Items);
		}
	}
}

void AVRegionServer::PruneInvalidConnections()
{
	for (auto It = ConnectionStates.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			It.RemoveCurrent();
		}
	}
}
