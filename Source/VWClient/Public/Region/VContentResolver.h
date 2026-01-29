#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMCollision.h"
#include "Model/Package/VMMaterial.h"
#include "UObject/Object.h"
#include "VContentResolver.generated.h"

class UDecalComponent;
class URegionClientBridge;
class UStaticMeshComponent;
class USceneComponent;
class UVCollisionPresenter;
class UVDecalPresenter;
class UVLightPresenter;
class UVMaterialPresenter;
class UVMeshPresenter;
class UVAssetManager;

UENUM(BlueprintType)
enum class EResolutionState : uint8
{
	Declared,
	Resolving,
	RenderableReady,
	Active,
	Retired
};

USTRUCT()
struct FResolvedSpatialInstance
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid SpatialId;

	UPROPERTY()
	TArray<FGuid> MaterialIds;

	UPROPERTY()
	TArray<FGuid> TextureIds;

	UPROPERTY()
	FGuid CollisionId;

	UPROPERTY()
	EResolutionState ResolutionState = EResolutionState::Declared;

	UPROPERTY()
	ESpatialItemType PayloadType = ESpatialItemType::Mesh;

	UPROPERTY()
	TWeakObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY()
	TWeakObjectPtr<UDecalComponent> DecalComponent;

	UPROPERTY()
	TWeakObjectPtr<USceneComponent> LightComponent;

	UPROPERTY()
	TWeakObjectPtr<USceneComponent> CollisionRoot;

	UPROPERTY()
	bool bRequestedMaterials = false;

	UPROPERTY()
	bool bRequestedCollision = false;

	UPROPERTY()
	bool bRequestedTextures = false;

	UPROPERTY()
	bool bMeshPresented = false;

	UPROPERTY()
	bool bMaterialPresented = false;

	UPROPERTY()
	bool bDecalOrLightPresented = false;

	UPROPERTY()
	bool bCollisionPresented = false;
};

UCLASS()
class VWCLIENT_API UVContentResolver : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		AActor* InOwner,
		URegionClientBridge* InRegionBridge,
		UVMeshPresenter* InMeshPresenter,
		UVMaterialPresenter* InMaterialPresenter,
		UVLightPresenter* InLightPresenter,
		UVDecalPresenter* InDecalPresenter,
		UVCollisionPresenter* InCollisionPresenter,
		UVAssetManager* InAssetManager);

	void Tick(float DeltaSeconds);

	void OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items);
	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);
	void OnCollisionsBatchReceived(const TArray<FVMCollision>& Collisions);
	void OnSpatialItemRemoved(const FGuid& ItemId);

	void DumpUnresolvedSpatialItems() const;
	void DumpActiveSpatialCount() const;

private:
	TWeakObjectPtr<AActor> OwnerActor;
	TWeakObjectPtr<URegionClientBridge> RegionBridge;
	TWeakObjectPtr<UVMeshPresenter> MeshPresenter;
	TWeakObjectPtr<UVMaterialPresenter> MaterialPresenter;
	TWeakObjectPtr<UVLightPresenter> LightPresenter;
	TWeakObjectPtr<UVDecalPresenter> DecalPresenter;
	TWeakObjectPtr<UVCollisionPresenter> CollisionPresenter;
	TWeakObjectPtr<UVAssetManager> AssetManager;

	TMap<FGuid, FVMSpatialItemNet> PendingSpatialData;
	TMap<FGuid, FVMMaterial> PendingMaterials;
	TMap<FGuid, FVMCollision> PendingCollisions;
	TMap<FGuid, FResolvedSpatialInstance> ResolvedInstances;

	TSet<FGuid> RequestedMaterialIds;
	TSet<FGuid> RequestedCollisionIds;
	TSet<FGuid> RequestedTextureIds;

	bool bMaterialRequestInFlight = false;
	bool bCollisionRequestInFlight = false;

	int32 MaxMaterialItemLimit = 64;
	int32 MaxCollisionItemLimit = 64;

	void UpdateOrCreateInstance(const FVMSpatialItemNet& Item);
	void EvaluateInstance(FResolvedSpatialInstance& Instance);
	void IssueDependencyRequests();
	void PresentSpatial(FResolvedSpatialInstance& Instance, const FVMSpatialItemNet& Item);
	void ReleaseSpatial(const FGuid& ItemId);
	void PrunePayloadsForInstance(const FResolvedSpatialInstance& Instance);

	void RegisterMeshComponent(const FGuid& ItemId, UStaticMeshComponent* MeshComponent);
	void RegisterCollisionComponent(const FGuid& ItemId, USceneComponent* CollisionRoot);

	void UpdateTextureDependencies(FResolvedSpatialInstance& Instance);
	bool AreMaterialsReady(const FResolvedSpatialInstance& Instance) const;
	bool AreTexturesReady(const FResolvedSpatialInstance& Instance) const;
	bool IsCollisionReady(const FResolvedSpatialInstance& Instance) const;
	bool CanRender(const FResolvedSpatialInstance& Instance) const;
	bool CanActivate(const FResolvedSpatialInstance& Instance) const;

	void LogStateTransition(const FResolvedSpatialInstance& Instance, EResolutionState NewState) const;
};
