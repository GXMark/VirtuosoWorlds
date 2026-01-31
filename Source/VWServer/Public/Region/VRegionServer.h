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
	void UpdateConnectionLocation(UNetConnection* Connection, const FVector& Location);

protected:
	virtual void BeginPlay() override;

private:
	AVSpatialItemActor* SpawnSpatialItemActor(const FVMSpatialItemNet& Item);
	void RegisterSpatialItemActor(AVSpatialItemActor* Actor, const FGuid& ItemId);
	FIntPoint GetCellCoord(const FVector& Location) const;
	void UpdateItemCell(const FGuid& ItemId, const FVector& Location);
	void RemoveItemFromCell(const FGuid& ItemId);
	TSet<FIntPoint> BuildCellWindow(const FIntPoint& Center, int32 Radius) const;
	void GatherItemsForCells(const TSet<FIntPoint>& Cells, TSet<FGuid>& OutItems) const;
	void PruneInvalidConnections();

	UPROPERTY()
	TMap<FGuid, FVMSpatialItemNet> SpatialItemState;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<AVSpatialItemActor>> SpatialItemActors;

	UPROPERTY(EditAnywhere, Category = "Relevancy")
	float CellSize = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Relevancy")
	int32 InnerCellRadius = 1;

	UPROPERTY(EditAnywhere, Category = "Relevancy")
	int32 OuterCellRadius = 2;

	TMap<FIntPoint, TSet<FGuid>> CellBuckets;
	TMap<FGuid, FIntPoint> ItemCells;

	struct FConnectionRelevancyState
	{
		bool bInitialized = false;
		FIntPoint CurrentCell = FIntPoint::ZeroValue;
		TSet<FIntPoint> ActiveCells;
	};

	TMap<TWeakObjectPtr<UNetConnection>, FConnectionRelevancyState> ConnectionStates;
};
