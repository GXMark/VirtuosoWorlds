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
