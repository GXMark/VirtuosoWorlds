#pragma once

#include "CoreMinimal.h"
#include "Region/VRegionClientJobs.h"
#include "VRegionClientResolver.generated.h"

class AActor;
class UVAssetManager;

UCLASS()
class VWCLIENT_API UVRegionClientResolver : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UVAssetManager* InAssetManager);
	void ConsumeJob(const FVRegionClientJob& Job);

private:
	struct FRegionClientItemState
	{
		TWeakObjectPtr<AActor> Actor;
		FMeshAssetId DesiredMeshAssetId;
		TArray<uint32> DesiredMaterialIdsBySlot;
		uint32 Generation = 0;
		uint8 AppliedMask = 0;
	};

	TMap<FGuid, FRegionClientItemState> ItemStates;
	TWeakObjectPtr<UVAssetManager> AssetManager;
};
