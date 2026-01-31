#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"
#include "UObject/Object.h"
#include "VRegionResolver.generated.h"

class URegionClientBridge;
class UVAssetManager;
class UMaterialInterface;
class UStaticMesh;

USTRUCT()
struct FResolvedItemBundle
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid ItemId;

	UPROPERTY()
	FGuid ParentId;

	UPROPERTY()
	ESpatialItemType ItemType = ESpatialItemType::Mesh;

	UPROPERTY()
	FTransform WorldTransform = FTransform::Identity;

	UPROPERTY()
	FVMMeshComponentNet MeshPayload;

	UPROPERTY()
	FVMPointLightComponentNet PointLightPayload;

	UPROPERTY()
	FVMSpotLightComponentNet SpotLightPayload;

	UPROPERTY()
	FVMDecalComponentNet DecalPayload;

	UPROPERTY()
	TObjectPtr<UStaticMesh> MeshAsset;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY()
	bool bTexturesReady = false;

};

USTRUCT()
struct FResolvedSpatialInstance
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid SpatialId;

	UPROPERTY()
	FVMSpatialItemNet SpatialItem;

	UPROPERTY()
	TArray<FGuid> MaterialIds;

	UPROPERTY()
	TArray<FGuid> TextureIds;

	UPROPERTY()
	ESpatialItemType PayloadType = ESpatialItemType::Mesh;

	UPROPERTY()
	bool bRequestedMaterials = false;


	UPROPERTY()
	bool bIssued = false;
};

UCLASS()
class VWCLIENT_API UVRegionResolver : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		URegionClientBridge* InRegionBridge,
		UVAssetManager* InAssetManager);

	void OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items);
	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);

	bool IsBundleReady(const FGuid& ItemId) const;

	void GetResolvedBundles(int32 MaxResolvedBundlesPerTick, TArray<FResolvedItemBundle>& OutBundles);

private:
	TWeakObjectPtr<URegionClientBridge> RegionBridge;
	TWeakObjectPtr<UVAssetManager> AssetManager;

	TMap<FGuid, FVMSpatialItemNet> PendingSpatialData;
	TMap<FGuid, FVMMaterial> PendingMaterials;
	TMap<FGuid, FResolvedSpatialInstance> ResolvedInstances;

	TArray<FGuid> SpatialOrder;
	TSet<FGuid> IssuedItemIds;

	TSet<FGuid> RequestedMaterialIds;
	TSet<FGuid> RequestedMeshIds;
	TSet<FGuid> RequestedTextureIds;

	bool bMaterialRequestInFlight = false;
	int32 MaxMaterialItemLimit = 64;

	void UpdateOrCreateInstance(const FVMSpatialItemNet& Item);
	void UpdateTextureDependencies(FResolvedSpatialInstance& Instance);
	void IssueDependencyRequests();

	bool AreMaterialsReady(const FResolvedSpatialInstance& Instance) const;
	bool AreTexturesReady(const FResolvedSpatialInstance& Instance) const;
	bool IsMeshReady(const FResolvedSpatialInstance& Instance) const;
	bool CanRender(const FResolvedSpatialInstance& Instance) const;
	bool CanActivate(const FResolvedSpatialInstance& Instance) const;
};
