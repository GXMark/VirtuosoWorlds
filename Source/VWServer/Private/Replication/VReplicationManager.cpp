#include "Replication/VReplicationManager.h"

#include "Engine/NetConnection.h"
#include "GameFramework/Actor.h"

void UVReplicationManager::RegisterSpatialItemActor(AActor* Actor, const FGuid& Id)
{
	if (!Actor || !Id.IsValid())
	{
		return;
	}

	RegisteredActors.Add(Id, Actor);
}

void UVReplicationManager::UnregisterSpatialItemActor(const FGuid& Id)
{
	if (!Id.IsValid())
	{
		return;
	}

	RegisteredActors.Remove(Id);
}

void UVReplicationManager::UnregisterSpatialItemActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	for (auto It = RegisteredActors.CreateIterator(); It; ++It)
	{
		if (It.Value().Get() == Actor)
		{
			It.RemoveCurrent();
			break;
		}
	}
}

void UVReplicationManager::SetConnectionRelevancy(UNetConnection* Connection, const TSet<FGuid>& Relevant)
{
	if (!Connection)
	{
		return;
	}

	PruneInvalidConnections();
	const TWeakObjectPtr<UNetConnection> Key(Connection);
	ConnectionRelevancy.FindOrAdd(Key) = Relevant;
}

void UVReplicationManager::AddRelevant(UNetConnection* Connection, const TSet<FGuid>& Relevant)
{
	if (!Connection)
	{
		return;
	}

	PruneInvalidConnections();
	const TWeakObjectPtr<UNetConnection> Key(Connection);
	TSet<FGuid>& Existing = ConnectionRelevancy.FindOrAdd(Key);
	Existing.Append(Relevant);
}

void UVReplicationManager::RemoveRelevant(UNetConnection* Connection, const TSet<FGuid>& Relevant)
{
	if (!Connection)
	{
		return;
	}

	PruneInvalidConnections();
	const TWeakObjectPtr<UNetConnection> Key(Connection);
	TSet<FGuid>* Existing = ConnectionRelevancy.Find(Key);
	if (!Existing)
	{
		return;
	}

	for (const FGuid& Id : Relevant)
	{
		Existing->Remove(Id);
	}
}

bool UVReplicationManager::IsSpatialItemRelevantForConnection(const FGuid& Id, const UNetConnection* Connection) const
{
	if (!Connection || !Id.IsValid())
	{
		return false;
	}

	const TWeakObjectPtr<UNetConnection> Key(const_cast<UNetConnection*>(Connection));
	const TSet<FGuid>* Relevant = ConnectionRelevancy.Find(Key);
	return Relevant && Relevant->Contains(Id);
}

bool UVReplicationManager::HasConnectionRelevancy(const UNetConnection* Connection) const
{
	if (!Connection)
	{
		return false;
	}

	const TWeakObjectPtr<UNetConnection> Key(const_cast<UNetConnection*>(Connection));
	return ConnectionRelevancy.Contains(Key);
}

AActor* UVReplicationManager::FindActorForId(const FGuid& Id) const
{
	if (!Id.IsValid())
	{
		return nullptr;
	}

	if (const TWeakObjectPtr<AActor>* Found = RegisteredActors.Find(Id))
	{
		return Found->Get();
	}

	return nullptr;
}

void UVReplicationManager::PruneInvalidConnections()
{
	for (auto It = ConnectionRelevancy.CreateIterator(); It; ++It)
	{
		if (!It.Key().IsValid())
		{
			It.RemoveCurrent();
		}
	}
}
