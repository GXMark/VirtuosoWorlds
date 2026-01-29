#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Network/VMGuidNet.h"
#include "VMaterialPresenter.generated.h"

class UStaticMeshComponent;
class UVAssetManager;

UCLASS()
class VWCLIENT_API UVMaterialPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UVAssetManager* InAssetManager);
	void ApplyMaterialsAsync(
		const FGuid& InItemId,
		UStaticMeshComponent* MeshComp,
		const TArray<FVMGuidNet>& InMaterialIds);
	void ForgetItem(const FGuid& InItemId);

private:
	UPROPERTY()
	TObjectPtr<UVAssetManager> AssetManager;

	TMap<FGuid, TWeakObjectPtr<UStaticMeshComponent>> MeshByItemId;
	TMap<FGuid, TArray<FGuid>> RequestedMaterialsByItemId;
};
