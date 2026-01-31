#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Package/VMMaterial.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "Region/VRegionClientJobs.h"
#include "Region/VRegionRenderQueue.h"
#include "VRegionClient.generated.h"

class URegionClientBridge;
class UVAssetManager;
class UVRegionClientResolver;
class UVMaterialPresenterApplier;
class UVMaterialResolver;

USTRUCT()
struct FSpatialActorVisualState
{
	GENERATED_BODY()

	FMeshAssetId MeshAssetId;
	TArray<uint32> MaterialIdsBySlot;
	TArray<int32> PendingMaterialSlots;
	bool bForceMaterialReapply = false;
	bool bMeshPending = true;
	bool bMaterialsPending = true;
};

UCLASS()
class VWCLIENT_API AVRegionClient : public AActor
{
	GENERATED_BODY()

public:
	AVRegionClient();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);

	static AVRegionClient* Get(const UObject* WorldContext);

private:
	struct FVRegionClientJobKey
	{
		FGuid ItemId;
		EVRegionClientJobType JobType = EVRegionClientJobType::SpatialActorObserved;

		friend bool operator==(const FVRegionClientJobKey& Left, const FVRegionClientJobKey& Right)
		{
			return Left.ItemId == Right.ItemId && Left.JobType == Right.JobType;
		}
	};

	friend uint32 GetTypeHash(const FVRegionClientJobKey& Key)
	{
		return HashCombine(GetTypeHash(Key.ItemId), GetTypeHash(static_cast<uint8>(Key.JobType)));
	}

	void HandleSpatialActorPostInit(AActor* Actor, const FSpatialItemId& SpatialId);
	void HandleMeshAssetIdChanged(AActor* Actor, const FMeshAssetId& MeshAssetId);
	void HandleMaterialIdsChanged(AActor* Actor, const TArray<uint32>& MaterialIds);

	UFUNCTION()
	void HandleActorDestroyed(AActor* Actor);

	void EnqueueJob(FVRegionClientJob&& Job);
	bool ShouldCoalesceJob(EVRegionClientJobType JobType) const;

	void RegisterActor(AActor* Actor, const FGuid& ItemId);
	FGuid ResolveSpatialId(AActor* Actor, const FSpatialItemId* SpatialIdOverride = nullptr);
	void SyncVisualStateFromActor(AActor* Actor, const FGuid& ItemId);
	void TryApplyMesh(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State) const;
	void TryApplyMaterials(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State) const;

	TMap<FGuid, TWeakObjectPtr<AActor>> SpatialItemActors;
	TMap<TWeakObjectPtr<AActor>, FGuid> ActorToSpatialId;
	TMap<FGuid, FSpatialActorVisualState> VisualStates;
	TArray<FVRegionClientJob> JobQueue;
	TMap<FVRegionClientJobKey, int32> CoalescedJobIndices;
	uint64 JobSequence = 0;
	FVRegionRenderQueue RenderQueue;

	TObjectPtr<UVAssetManager> AssetManager = nullptr;
	TObjectPtr<UVRegionClientResolver> RegionClientResolver = nullptr;
	TObjectPtr<UVMaterialResolver> MaterialResolver = nullptr;
	TObjectPtr<UVMaterialPresenterApplier> MaterialPresenter = nullptr;
	TObjectPtr<URegionClientBridge> RegionBridge = nullptr;

	FDelegateHandle PostInitHandle;
	FDelegateHandle MeshAssetHandle;
	FDelegateHandle MaterialIdsHandle;
};
