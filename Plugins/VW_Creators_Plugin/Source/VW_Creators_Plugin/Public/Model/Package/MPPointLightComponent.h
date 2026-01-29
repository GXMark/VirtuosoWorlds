#pragma once

#include "CoreMinimal.h"
#include "MPPointLightComponent.generated.h"

USTRUCT()
struct FMPPointLightComponent
{
	GENERATED_BODY()

	UPROPERTY()
	float intensity;

	UPROPERTY()
	FName units;

	UPROPERTY()
	FColor color;

	UPROPERTY()
	float attenuation_radius;

	UPROPERTY()
	float source_radius;
	
	UPROPERTY()
	float soft_source_radius;
	
	UPROPERTY()
	float source_length;
	
	UPROPERTY()
	bool use_temperature;

	UPROPERTY()
	float temperature;

	UPROPERTY()
	bool affects_world;

	UPROPERTY()
	bool cast_shadows;

	UPROPERTY()
	float indirect_light_intensity;

	UPROPERTY()
	float volumetric_scatter_intensity;

	FMPPointLightComponent(): 
		intensity(0)
		, color()
		, attenuation_radius(0)
		, source_radius(0)
		, soft_source_radius(0)
		, source_length(0)
		, use_temperature(false)
		, temperature(0)
		, affects_world(false)
		, cast_shadows(false)
		, indirect_light_intensity(0)
		, volumetric_scatter_intensity(0)
	{}
	
	FMPPointLightComponent(const float& InIntensity, const FName& InUnits, const FColor& InColor, const float& InAttenuationRadius, const float& InSourceRadius, const float& InSoftSourceRadius, const float& InSourceLength, const bool& InUseTemperature, const float& InTemperature, const bool& InAffectsWorld, const bool& InCastShadows, const float& InIndirectLightIntensity, const float& InVolumetricScatterIntensity):
		intensity(InIntensity)
		, units(InUnits)
		, color(InColor)
		, attenuation_radius(InAttenuationRadius)
		, source_radius(InSourceRadius)
		, soft_source_radius(InSoftSourceRadius)
		, source_length(InSourceLength)
		, use_temperature(InUseTemperature)
		, temperature(InTemperature)
		, affects_world(InAffectsWorld)
		, cast_shadows(InCastShadows)
		, indirect_light_intensity(InIndirectLightIntensity)
		, volumetric_scatter_intensity(InVolumetricScatterIntensity)
	{}
};