// VirtuosoWorlds/Public/Region/VRegionClient.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"
#include "Region/VRegionPresenter.h"
#include "Region/VRegionResolver.h"
#include "Region/VRegionClientBridge.h"
#include "VRegionClient.generated.h"

class USceneComponent;
class UVMaterialPresenter;
class UVMeshPresenter;
class UVAssetManager;
class UVRegionResolver;
class UVRegionPresenter;

UCLASS()
class VWCLIENT_API AVRegionClient : public AActor
{
	GENERATED_BODY()

public:
	AVRegionClient();

	// Called by the local player controller to update streaming state.
	void OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items, bool bHasMore);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TObjectPtr<UVMeshPresenter> MeshPresenter;

	UPROPERTY()
	TObjectPtr<UVMaterialPresenter> MaterialPresenter;

	UPROPERTY()
	TObjectPtr<UVRegionResolver> RegionResolver;

	UPROPERTY()
	TObjectPtr<UVRegionPresenter> RegionPresenter;

	UPROPERTY()
	TObjectPtr<URegionClientBridge> RegionBridge;

	// Cached asset manager pointer (world subsystem)
	UPROPERTY()
	TObjectPtr<UVAssetManager> AssetManager;

	// --- Client spatial streaming state (driven by RegionClient tick) ---
	int32 MaxResolvedBundlesPerTick = 64;

	TArray<TArray<FVMSpatialItemNet>> PendingSpatialBatches;
	TArray<TArray<FVMMaterial>> PendingMaterialBatches;
public:
	// Called by the local player controller when a material batch response arrives.
	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);
};
