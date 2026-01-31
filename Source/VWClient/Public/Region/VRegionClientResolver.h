#pragma once

#include "CoreMinimal.h"
#include "Region/VRegionClientJobs.h"
#include "Region/VRegionRenderQueue.h"
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
	void EmitRenderWork(FVRegionRenderQueue& RenderQueue, UVAssetManager* AssetManager);

	struct FRegionClientItemSnapshot
	{
		TWeakObjectPtr<AActor> Actor;
		FMeshAssetId MeshAssetId;
		TArray<uint32> MaterialIdsBySlot;
		uint32 Generation = 0;
	};

	bool GetItemSnapshot(const FGuid& ItemId, FRegionClientItemSnapshot& OutSnapshot) const;
	void MarkApplied(const FGuid& ItemId, EVRegionRenderWorkType WorkType);
	bool MarkLegacyFallbackAttempted(const FGuid& ItemId, EVRegionRenderWorkType WorkType);

private:
	struct FRegionClientItemState
	{
		TWeakObjectPtr<AActor> Actor;
		FMeshAssetId DesiredMeshAssetId;
		TArray<uint32> DesiredMaterialIdsBySlot;
		uint32 Generation = 0;
		uint8 AppliedMask = 0;
		uint8 LegacyFallbackMask = 0;
	};

	TMap<FGuid, FRegionClientItemState> ItemStates;
	TSet<FGuid> DirtyItemIds;
	TWeakObjectPtr<UVAssetManager> AssetManager;
};
