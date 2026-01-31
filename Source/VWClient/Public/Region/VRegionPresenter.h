#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Region/VRegionResolver.h"
#include "VRegionPresenter.generated.h"

class UVMaterialPresenter;
class UVMeshPresenter;

UCLASS()
class VWCLIENT_API UVRegionPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		UVMeshPresenter* InMeshPresenter,
		UVMaterialPresenter* InMaterialPresenter);

	void Commit(const TArray<FResolvedItemBundle>& Bundles);
	void ReleaseItem(const FGuid& ItemId);

private:
	TWeakObjectPtr<UVMeshPresenter> MeshPresenter;
	TWeakObjectPtr<UVMaterialPresenter> MaterialPresenter;

	void PresentBundle(const FResolvedItemBundle& Bundle);
};
