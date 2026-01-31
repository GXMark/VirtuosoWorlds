#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "Replication/VReplicationManager.h"
#include "VRegionServer.generated.h"

class AVSpatialItemActor;
class UNetConnection;

UCLASS()
class VWSERVER_API AVRegionServer : public AActor
{
	GENERATED_BODY()

public:
	AVRegionServer();

	void LoadSpatialItems(const TArray<FVMSpatialItemNet>& Items);
	void UpsertSpatialItem(const FVMSpatialItemNet& Item);
	void RemoveSpatialItem(const FSpatialItemId& ItemId);
	void UpdateConnectionRelevancy(UNetConnection* Connection, const TSet<FSpatialItemId>& RelevantItems);

protected:
	virtual void BeginPlay() override;

private:
	AVSpatialItemActor* SpawnSpatialItemActor(const FVMSpatialItemNet& Item);
	void RegisterSpatialItemActor(AVSpatialItemActor* Actor, const FSpatialItemId& ItemId);

	UPROPERTY()
	TMap<FSpatialItemId, FVMSpatialItemNet> SpatialItemState;

	UPROPERTY()
	TMap<FSpatialItemId, TObjectPtr<AVSpatialItemActor>> SpatialItemActors;
};
