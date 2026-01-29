#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VMaterialPresenter.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class VWCLIENT_API UVMaterialPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();
	void ApplyMaterials(
		const FGuid& InItemId,
		UStaticMeshComponent* MeshComp,
		const TArray<UMaterialInterface*>& InMaterials);
	void ForgetItem(const FGuid& InItemId);

private:
	TMap<FGuid, TWeakObjectPtr<UStaticMeshComponent>> MeshByItemId;
	TMap<FGuid, TArray<UMaterialInterface*>> AppliedMaterialsByItemId;
};
