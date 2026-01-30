#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Region/VRegionResolver.h"
#include "VRegionPresenter.generated.h"

class UVCollisionPresenter;
class UVDecalPresenter;
class UVLightPresenter;
class UVMaterialPresenter;
class UVMeshPresenter;

UCLASS()
class VWCLIENT_API UVRegionPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		UVMeshPresenter* InMeshPresenter,
		UVMaterialPresenter* InMaterialPresenter,
		UVLightPresenter* InLightPresenter,
		UVDecalPresenter* InDecalPresenter,
		UVCollisionPresenter* InCollisionPresenter);

	void Commit(const TArray<FResolvedItemBundle>& Bundles);
	void ReleaseItem(const FGuid& ItemId);

private:
	TWeakObjectPtr<UVMeshPresenter> MeshPresenter;
	TWeakObjectPtr<UVMaterialPresenter> MaterialPresenter;
	TWeakObjectPtr<UVLightPresenter> LightPresenter;
	TWeakObjectPtr<UVDecalPresenter> DecalPresenter;
	TWeakObjectPtr<UVCollisionPresenter> CollisionPresenter;

	void PresentBundle(const FResolvedItemBundle& Bundle);
};
