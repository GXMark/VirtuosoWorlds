// VirtuosoWorlds/Public/Region/VRegionClient.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"
#include "Model/Package/VMCollision.h"
#include "Region/VRegionPresenter.h"
#include "Region/VRegionResolver.h"
#include "Region/VRegionClientBridge.h"
#include "VRegionClient.generated.h"

class USceneComponent;
class UVDecalPresenter;
class UVLightPresenter;
class UVMaterialPresenter;
class UVMeshPresenter;
class UVAssetManager;
class UVCollisionPresenter;
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

	// Enable/disable client-driven spatial streaming.
	void SetSpatialStreamingEnabled(bool bEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TObjectPtr<USceneComponent> CollisionRoot;

	UPROPERTY()
	TObjectPtr<UVMeshPresenter> MeshPresenter;

	UPROPERTY()
	TObjectPtr<UVMaterialPresenter> MaterialPresenter;

	UPROPERTY()
	TObjectPtr<UVLightPresenter> LightPresenter;

	UPROPERTY()
	TObjectPtr<UVDecalPresenter> DecalPresenter;

	UPROPERTY()
	TObjectPtr<UVCollisionPresenter> CollisionPresenter;

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
	bool bSpatialStreamActive = false;
	bool bSpatialRequestInFlight = false;
	bool bSpatialHasMore = true;
	FVector SpatialOrigin = FVector::ZeroVector;
	// When player moves farther than this from SpatialOrigin, request priority recenters.
	float SpatialRecenterDistanceCm = 5000.f;
	// Optional: only stream spatial items within this radius. Set <= 0 to disable.
	// If we move while a request is in flight, defer the recenter until the response arrives.
	bool bSpatialRecenterPending = false;
	FVector PendingSpatialOrigin = FVector::ZeroVector;
	int32 MaxSpatialItemsPerRequest = 64;
	int32 MaxResolvedBundlesPerTick = 64;

	// Helper to issue the next spatial request via the owning controller interface.
	void RequestNextSpatialBatch();

	TArray<TArray<FVMSpatialItemNet>> PendingSpatialBatches;
	TArray<TArray<FVMMaterial>> PendingMaterialBatches;
	TArray<TArray<FVMCollision>> PendingCollisionBatches;

public:
	// Called by the local player controller when a material batch response arrives.
	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);

	// Called by the local player controller when a collision batch response arrives.
	void OnCollisionsBatchReceived(const TArray<FVMCollision>& Collisions);

	// Called when a spatial item unloads.
	void OnSpatialItemRemoved(const FGuid& ItemId);
};
