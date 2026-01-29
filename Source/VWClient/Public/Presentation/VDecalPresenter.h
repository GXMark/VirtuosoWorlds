#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Network/VMDecalComponentNet.h"
#include "VDecalPresenter.generated.h"

class UDecalComponent;
class UVAssetManager;

UCLASS()
class VWCLIENT_API UVDecalPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwner, USceneComponent* InPresentationRoot, UVAssetManager* InAssetManager);

	void PresentDecalItem(
		const FGuid& InItemId,
		const FVMDecalComponentNet& InDecalData,
		const FTransform& InWorldTransform);

	void DestroyItem(const FGuid& InItemId);

private:
	TWeakObjectPtr<AActor> PresentationOwner;
	TWeakObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TObjectPtr<UVAssetManager> AssetManager;

	TMap<FGuid, TObjectPtr<UDecalComponent>> SpawnedComponents;
	TMap<FGuid, FGuid> RequestedMaterialsByItemId;
};
