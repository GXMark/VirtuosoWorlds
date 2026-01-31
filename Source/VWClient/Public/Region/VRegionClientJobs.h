#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"
#include "VRegionClientJobs.generated.h"

class AActor;

UENUM()
enum class EVRegionClientJobType : uint8
{
	SpatialActorObserved,
	MeshAssetIdChanged,
	MaterialIdsChanged,
	ActorDestroyed,
	MaterialsBatchReceived,
};

USTRUCT()
struct FVRegionClientJob
{
	GENERATED_BODY()

	EVRegionClientJobType JobType = EVRegionClientJobType::SpatialActorObserved;
	FGuid ItemId;
	FMeshAssetId MeshAssetId;
	TArray<uint32> MaterialIdsBySlot;
	TWeakObjectPtr<AActor> Actor;
	TArray<FVMMaterial> MaterialsBatch;
	uint64 Sequence = 0;
};
