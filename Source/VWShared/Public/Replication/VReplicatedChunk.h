// VReplicatedChunk.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/Chunk/VReplicatedSpatialFastArray.h"
#include "Model/Network/VMGuidNet.h"
#include "VReplicatedChunk.generated.h"

// Data-only delegates (C++ only; no Blueprint dependency)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpatialItemAdded, const FVMSpatialItemNet&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpatialItemUpdated, const FVMSpatialItemNet&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpatialItemRemoved, const FGuid&);

UCLASS()
class VWSHARED_API AVReplicatedChunk : public AActor
{
	GENERATED_BODY()

public:
	AVReplicatedChunk();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Chunk")
	FVMGuidNet ChunkID;

	UPROPERTY(Replicated)
	FVReplicatedSpatialItemArray SpatialItems;

	// Server API
	void ServerAddOrUpdateSpatialItem(const FVMSpatialItemNet& InItem);
	void ServerRemoveSpatialItem(const FVMGuidNet& InItemID);

	// Data-only signals (client presenters subscribe here)
	FOnSpatialItemAdded OnItemAdded;
	FOnSpatialItemUpdated OnItemUpdated;
	FOnSpatialItemRemoved OnItemRemoved;

	// Called by FastArray callbacks (data-only)
	void HandleSpatialItemAdded(const FVMSpatialItemNet& InItem) const;
	void HandleSpatialItemChanged(const FVMSpatialItemNet& InItem) const;
	void HandleSpatialItemRemoved(const FVMGuidNet& InItemID) const;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	int32 FindIndexByItemID(const FVMGuidNet& InItemID) const;
};
