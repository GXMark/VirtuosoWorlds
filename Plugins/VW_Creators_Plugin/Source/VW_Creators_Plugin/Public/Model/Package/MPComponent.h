#pragma once

#include "CoreMinimal.h"
#include "MPDecalComponent.h"
#include "MPSpotLightComponent.h"
#include "MPMeshComponent.h"
#include "MPPointLightComponent.h"
#include "MPComponent.generated.h"

USTRUCT()
struct VW_CREATORS_PLUGIN_API FMPComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	FMPMeshComponent mesh_comp; // mesh component

	UPROPERTY()
	FMPDecalComponent decal_comp; // decal component

	UPROPERTY()
	FMPPointLightComponent point_light_comp; // point light component

	UPROPERTY()
	FMPSpotLightComponent spot_light_comp; // spot light component

	FMPComponent() {}

	FMPComponent(const FMPMeshComponent& StaticMeshComp, const FMPDecalComponent& DecalComp, const FMPPointLightComponent& PointLightComp, const FMPSpotLightComponent& SpotLightComp):
		mesh_comp(StaticMeshComp)
		, decal_comp(DecalComp)
		, point_light_comp(PointLightComp)
		, spot_light_comp(SpotLightComp)
	{}
};