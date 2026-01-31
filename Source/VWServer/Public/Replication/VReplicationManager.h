#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VReplicationManager.generated.h"

class UNetConnection;

UCLASS()
class VWSERVER_API UVReplicationManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterSpatialItemActor(AActor* Actor, const FGuid& Id);
	void UnregisterSpatialItemActor(const FGuid& Id);
	void UnregisterSpatialItemActor(AActor* Actor);
	void SetConnectionRelevancy(UNetConnection* Connection, const TSet<FGuid>& Relevant);
	void AddRelevant(UNetConnection* Connection, const TSet<FGuid>& Relevant);
	void RemoveRelevant(UNetConnection* Connection, const TSet<FGuid>& Relevant);
	bool IsSpatialItemRelevantForConnection(const FGuid& Id, const UNetConnection* Connection) const;
	bool HasConnectionRelevancy(const UNetConnection* Connection) const;
	AActor* FindActorForId(const FGuid& Id) const;

private:
	void PruneInvalidConnections();

	TMap<FGuid, TWeakObjectPtr<AActor>> RegisteredActors;
	TMap<TWeakObjectPtr<UNetConnection>, TSet<FGuid>> ConnectionRelevancy;
};
