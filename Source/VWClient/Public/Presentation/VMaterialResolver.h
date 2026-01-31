#pragma once

#include "CoreMinimal.h"
#include "Model/Package/VMMaterial.h"
#include "UObject/Object.h"
#include "VMaterialResolver.generated.h"

class URegionClientBridge;
class UVAssetManager;
class UMaterialInterface;
class UMaterialInstanceDynamic;

DECLARE_DELEGATE_TwoParams(FVOnMaterialResolved, uint32 /*MaterialId*/, UMaterialInterface* /*Material*/);

UCLASS()
class VWCLIENT_API UVMaterialResolver : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UVAssetManager* InAssetManager, URegionClientBridge* InRegionBridge);
	void ResolveMaterial(uint32 MaterialId, FVOnMaterialResolved Callback);
	void OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials);

private:
	void HandleMaterialResolved(UMaterialInstanceDynamic* Material, uint32 MaterialId);
	FGuid ResolveMaterialGuid(uint32 MaterialId) const;

	TMap<uint32, TObjectPtr<UMaterialInterface>> MaterialCacheById;
	TMap<uint32, TArray<FVOnMaterialResolved>> PendingCallbacksById;
	TSet<uint32> PendingMaterialRequests;

	TObjectPtr<UVAssetManager> AssetManager = nullptr;
	TObjectPtr<URegionClientBridge> RegionBridge = nullptr;
};
