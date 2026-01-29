#pragma once

#include "CoreMinimal.h"
#include "VMPointLightComponent.generated.h"

USTRUCT()
struct VWSHARED_API FVMPointLightComponent
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

	FVMPointLightComponent(): 
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
	
	FVMPointLightComponent(const float& Intensity, const FName& Units, const FColor& Color, const float& AttenuationRadius, const float& SourceRadius, const float& SoftSourceRadius, const float& SourceLength, const bool& UseTemperature, const float& Temperature, const bool& AffectsWorld, const bool& CastShadows, const float& IndirectLightIntensity, const float& VolumetricScatterIntensity):
		intensity(Intensity)
		, units(Units)
		, color(Color)
		, attenuation_radius(AttenuationRadius)
		, source_radius(SourceRadius)
		, soft_source_radius(SoftSourceRadius)
		, source_length(SourceLength)
		, use_temperature(UseTemperature)
		, temperature(Temperature)
		, affects_world(AffectsWorld)	
		, cast_shadows(CastShadows)
		, indirect_light_intensity(IndirectLightIntensity)
		, volumetric_scatter_intensity(VolumetricScatterIntensity)
	{}
};