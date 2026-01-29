#pragma once
#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "Model/Package/VMPointLightComponent.h"
#include "Model/Package/VMSpotLightComponent.h"
#include "VLightAgent.generated.h"

UCLASS()
class VWCLIENT_API UVLightAgent : public UObject
{
	GENERATED_BODY()
public:
	bool Initialize();
	
	static void DeserializePointLightData(const FVMPointLightComponent& InPointLightItem, UPointLightComponent* OutPointLightComponent);
	static void SerializePointLightData(const UPointLightComponent* InPointLightComponent, FVMPointLightComponent& OutPointLightItem);
	
	static void DeserializeSpotLightData(const FVMSpotLightComponent& InSpotLightItem, USpotLightComponent* OutSpotLightComponent);
	static void SerializeSpotLightData(const USpotLightComponent* InSpotLightComponent, FVMSpotLightComponent& OutSpotLightItem);
};