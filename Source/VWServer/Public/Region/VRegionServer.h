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
	void RemoveSpatialItem(const FGuid& ItemId);
	void UpdateConnectionRelevancy(UNetConnection* Connection, const TSet<FGuid>& RelevantItems);

protected:
	virtual void BeginPlay() override;

private:
	AVSpatialItemActor* SpawnSpatialItemActor(const FVMSpatialItemNet& Item);
	void RegisterSpatialItemActor(AVSpatialItemActor* Actor, const FGuid& ItemId);

	UPROPERTY()
	TMap<FGuid, FVMSpatialItemNet> SpatialItemState;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<AVSpatialItemActor>> SpatialItemActors;
};
