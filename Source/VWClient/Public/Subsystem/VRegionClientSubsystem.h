#pragma once

#include "CoreMinimal.h"
#include "Model/Package/VMMaterial.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "Subsystems/WorldSubsystem.h"
#include "VRegionClientSubsystem.generated.h"

class URegionClientBridge;
class UVAssetManager;
class UVMaterialPresenter;
class UMaterialInterface;
class UMaterialInstanceDynamic;

USTRUCT()
struct FSpatialActorVisualState
{
	GENERATED_BODY()

	FMeshAssetId MeshAssetId;
	TArray<uint32> MaterialIdsBySlot;
	TArray<TObjectPtr<UMaterialInterface>> ResolvedMaterials;
	int32 PendingMaterialRequests = 0;
	int32 MaterialRevision = 0;
	bool bMeshPending = true;
	bool bMaterialsPending = true;
};

UCLASS()
class VWCLIENT_API URegionClientSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);

private:
	void HandleSpatialActorPostInit(AActor* Actor, const FSpatialItemId& SpatialId);
	void HandleMeshAssetIdChanged(AActor* Actor, const FMeshAssetId& MeshAssetId);
	void HandleMaterialIdsChanged(AActor* Actor, const TArray<uint32>& MaterialIds);

	UFUNCTION()
	void HandleActorDestroyed(AActor* Actor);

	void RegisterActor(AActor* Actor, const FGuid& ItemId);
	FGuid ResolveSpatialId(AActor* Actor, const FSpatialItemId* SpatialIdOverride = nullptr);
	void SyncVisualStateFromActor(AActor* Actor, const FGuid& ItemId);
	void RequestMaterialUpdates(const FGuid& ItemId, FSpatialActorVisualState& State);
	void TryApplyMesh(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State);
	void TryApplyMaterials(const FGuid& ItemId, AActor* Actor, FSpatialActorVisualState& State);
	void OnMaterialResolved(UMaterialInstanceDynamic* Material, FGuid ItemId, int32 Revision, int32 SlotIndex);
	FGuid ResolveMaterialGuid(uint32 MaterialId) const;

	TMap<FGuid, TWeakObjectPtr<AActor>> SpatialItemActors;
	TMap<TWeakObjectPtr<AActor>, FGuid> ActorToSpatialId;
	TMap<FGuid, FSpatialActorVisualState> VisualStates;

	TObjectPtr<UVAssetManager> AssetManager = nullptr;
	TObjectPtr<UVMaterialPresenter> MaterialPresenter = nullptr;
	TObjectPtr<URegionClientBridge> RegionBridge = nullptr;

	FDelegateHandle PostInitHandle;
	FDelegateHandle MeshAssetHandle;
	FDelegateHandle MaterialIdsHandle;
};
