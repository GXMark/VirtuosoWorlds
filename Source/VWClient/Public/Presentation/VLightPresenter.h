#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Network/VMPointLightComponentNet.h"
#include "Model/Network/VMSpotLightComponentNet.h"
#include "VLightPresenter.generated.h"

class UPointLightComponent;
class USpotLightComponent;

UCLASS()
class VWCLIENT_API UVLightPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwner, USceneComponent* InPresentationRoot);

	void PresentPointLightItem(
		const FGuid& InItemId,
		const FVMPointLightComponentNet& InLightData,
		const FTransform& InWorldTransform);

	void PresentSpotLightItem(
		const FGuid& InItemId,
		const FVMSpotLightComponentNet& InLightData,
		const FTransform& InWorldTransform);

	void DestroyItem(const FGuid& InItemId);

private:
	TWeakObjectPtr<AActor> PresentationOwner;
	TWeakObjectPtr<USceneComponent> PresentationRoot;

	TMap<FGuid, TObjectPtr<USceneComponent>> SpawnedComponents;

	UPointLightComponent* GetOrCreatePointLight(const FGuid& InItemId);
	USpotLightComponent* GetOrCreateSpotLight(const FGuid& InItemId);

	static void ApplyPointLightPayload(UPointLightComponent* LightComp, const FVMPointLightComponentNet& InLightData);
	static void ApplySpotLightPayload(USpotLightComponent* LightComp, const FVMSpotLightComponentNet& InLightData);
};
