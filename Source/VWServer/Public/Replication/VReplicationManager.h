#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VReplicationManager.generated.h"

class UNetConnection;

using FSpatialItemId = FGuid;

UCLASS()
class VWSERVER_API UVReplicationManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterSpatialItemActor(AActor* Actor, const FSpatialItemId& Id);
	void UnregisterSpatialItemActor(const FSpatialItemId& Id);
	void UnregisterSpatialItemActor(AActor* Actor);
	void SetConnectionRelevancy(UNetConnection* Connection, const TSet<FSpatialItemId>& Relevant);
	void AddRelevant(UNetConnection* Connection, const TSet<FSpatialItemId>& Relevant);
	void RemoveRelevant(UNetConnection* Connection, const TSet<FSpatialItemId>& Relevant);
	bool IsSpatialItemRelevantForConnection(const FSpatialItemId& Id, const UNetConnection* Connection) const;
	bool HasConnectionRelevancy(const UNetConnection* Connection) const;
	AActor* FindActorForId(const FSpatialItemId& Id) const;

private:
	void PruneInvalidConnections();

	TMap<FSpatialItemId, TWeakObjectPtr<AActor>> RegisteredActors;
	TMap<TWeakObjectPtr<UNetConnection>, TSet<FSpatialItemId>> ConnectionRelevancy;
};
