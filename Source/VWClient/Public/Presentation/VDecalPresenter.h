#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Network/VMDecalComponentNet.h"
#include "VDecalPresenter.generated.h"

class UDecalComponent;
class UMaterialInterface;

UCLASS()
class VWCLIENT_API UVDecalPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwner, USceneComponent* InPresentationRoot);

	void PresentDecalItem(
		const FGuid& InItemId,
		const FVMDecalComponentNet& InDecalData,
		const FTransform& InWorldTransform,
		UMaterialInterface* InMaterial);

	UDecalComponent* FindDecalComponent(const FGuid& InItemId) const;

	void DestroyItem(const FGuid& InItemId);

private:
	TWeakObjectPtr<AActor> PresentationOwner;
	TWeakObjectPtr<USceneComponent> PresentationRoot;

	TMap<FGuid, TObjectPtr<UDecalComponent>> SpawnedComponents;
};
