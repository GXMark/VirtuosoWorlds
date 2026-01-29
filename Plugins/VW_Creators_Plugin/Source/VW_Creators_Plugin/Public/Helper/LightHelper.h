#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Model/Package/MPActor.h"
#include "Model/Package/MPComponent.h"

class VW_CREATORS_PLUGIN_API FCPLightHelper
{
public:
	static bool SerializePointLightComponent(const UPointLightComponent* InComponent, FMPActor& OutActorItem);
	static bool SerializeSpotLightComponent(const USpotLightComponent* InComponent, FMPActor& OutActorItem);
};
