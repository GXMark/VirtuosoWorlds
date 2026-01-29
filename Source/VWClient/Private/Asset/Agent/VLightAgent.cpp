#include "Asset/Agent/VLightAgent.h"
#include "Components/SpotLightComponent.h"
#include "Model/Package/VMPointLightComponent.h"
#include "Model/Package/VMSpotLightComponent.h"

bool UVLightAgent::Initialize()
{
	return true;
}

/**
 * Deserialize the point light item to the point light component
 * @param InPointLightItem - Point light component data 
 * @param OutPointLightComponent - Point light component
 */
void UVLightAgent::DeserializePointLightData(const FVMPointLightComponent& InPointLightItem, UPointLightComponent* OutPointLightComponent)
{
	// The total brightness or energy emitted by the light.
	OutPointLightComponent->Intensity = InPointLightItem.intensity;

	// Set the light unit scale
	if (InPointLightItem.units == "Candelas")
		OutPointLightComponent->SetIntensityUnits(ELightUnits::Candelas);
	else if (InPointLightItem.units == "Lumens")
		OutPointLightComponent->SetIntensityUnits(ELightUnits::Lumens);
	else
		OutPointLightComponent->SetIntensityUnits(ELightUnits::Unitless);

	// The colour of the light (RGB or via colour picker)
	OutPointLightComponent->SetLightColor(InPointLightItem.color);
	
	// Defines the maximum distance (radius) the light influences — beyond this, it won’t affect objects.
	OutPointLightComponent->SetAttenuationRadius(InPointLightItem.attenuation_radius);
	
	// The “size” of the light source. A larger value simulates a bigger bulb, producing softer, more diffuse shadows.
	OutPointLightComponent->SetSourceRadius(InPointLightItem.source_radius);
	
	// A softened version of source radius (shadow softness control)
	OutPointLightComponent->SetSoftSourceRadius(InPointLightItem.soft_source_radius);
	
	// If you want a stretched or elongated source (like a tube), you can give the light some length — useful for stylized or certain lighting setups.
	OutPointLightComponent->SetSourceLength(InPointLightItem.source_length);
	
	// When true uses color temperature (Kelvin) to determine its color
	OutPointLightComponent->SetUseTemperature(InPointLightItem.use_temperature);
	
	// Set the temperature e.g. 2000K-warm orange, 6500K - neutral white, 10000K - blue tinted cold
	OutPointLightComponent->SetTemperature(InPointLightItem.temperature);
	
	// Determines whether the light actually contributes lighting in the world e.g. illuminates surfaces, affects characters, shows up in reflections etc
	OutPointLightComponent->bAffectsWorld = InPointLightItem.affects_world;
	
	// Control whether static or dynamic shadows are cast (depending on mobility).
	OutPointLightComponent->CastShadows = InPointLightItem.cast_shadows ? 1 : 0;
	
	// How much light contributes to indirect (bounce) lighting, not direct light (part of GI)
	OutPointLightComponent->SetIndirectLightingIntensity(InPointLightItem.indirect_light_intensity);
	
	// Controls how strongly the light interacts with volumetric effects such as fog, mist, dust, smoke, god rays etc
	OutPointLightComponent->SetVolumetricScatteringIntensity(InPointLightItem.volumetric_scatter_intensity);

	OutPointLightComponent->MarkRenderStateDirty();
}

/**
 * Serialize the point light component into the point light component item
 * @param InPointLightComponent - Point light component
 * @param OutPointLightItem  - Point light component item
 */
void UVLightAgent::SerializePointLightData(const UPointLightComponent* InPointLightComponent, FVMPointLightComponent& OutPointLightItem)
{
	OutPointLightItem.intensity = InPointLightComponent->Intensity;
	
	if (InPointLightComponent->GetLightUnits() == ELightUnits::Candelas)
		OutPointLightItem.units = "Candelas";
	else if (InPointLightComponent->GetLightUnits() == ELightUnits::Lumens)
		OutPointLightItem.units = "Lumens";
	else
		OutPointLightItem.units = "Unitless";
	
	OutPointLightItem.color = InPointLightComponent->LightColor;
	OutPointLightItem.attenuation_radius = InPointLightComponent->AttenuationRadius;
	OutPointLightItem.source_radius = InPointLightComponent->SourceRadius;
	OutPointLightItem.soft_source_radius = InPointLightComponent->SoftSourceRadius;
	OutPointLightItem.source_length = InPointLightComponent->SourceLength;
	OutPointLightItem.use_temperature = InPointLightComponent->bUseTemperature;
	OutPointLightItem.temperature = InPointLightComponent->Temperature;
	OutPointLightItem.affects_world = InPointLightComponent->bAffectsWorld;
	OutPointLightItem.cast_shadows = InPointLightComponent->CastShadows;
	OutPointLightItem.indirect_light_intensity = InPointLightComponent->IndirectLightingIntensity;
	OutPointLightItem.volumetric_scatter_intensity = InPointLightComponent->VolumetricScatteringIntensity;
}

/**
 * Deserialize the spot light item to the point light component
 * @param InSpotLightItem - Spotlight component properties
 * @param OutSpotLightComponent - Spotlight component
 */
void UVLightAgent::DeserializeSpotLightData(const FVMSpotLightComponent& InSpotLightItem, USpotLightComponent* OutSpotLightComponent)
{
	// The total brightness or energy emitted by the light.
	OutSpotLightComponent->Intensity = InSpotLightItem.intensity;

	// Set the light unit scale
	if (InSpotLightItem.units == "Candelas")
		OutSpotLightComponent->SetIntensityUnits(ELightUnits::Candelas);
	else if (InSpotLightItem.units == "Lumens")
		OutSpotLightComponent->SetIntensityUnits(ELightUnits::Lumens);
	else
		OutSpotLightComponent->SetIntensityUnits(ELightUnits::Unitless);

	// The colour of the light (RGB or via colour picker)
	OutSpotLightComponent->SetLightColor(InSpotLightItem.color);
	
	// Defines the maximum distance (radius) the light influences — beyond this, it won’t affect objects.
	OutSpotLightComponent->SetAttenuationRadius(InSpotLightItem.attenuation_radius);
	
	// The fully bright core of the spotlight falling off smoothly from inner to outer
	OutSpotLightComponent->SetInnerConeAngle(InSpotLightItem.inner_cone_angle);
	
	// The maximum extent of the light, overall width of the cone, beyond this angle the light is zero
	OutSpotLightComponent->SetOuterConeAngle(InSpotLightItem.outer_cone_angle);
	
	// Defines the maximum distance (radius) the light influences — beyond this, it won’t affect objects. 
	OutSpotLightComponent->SetAttenuationRadius(InSpotLightItem.attenuation_radius);
	
	// The “size” of the light source. A larger value simulates a bigger bulb, producing softer, more diffuse shadows.
	OutSpotLightComponent->SetSourceRadius(InSpotLightItem.source_radius);
	
	// A softened version of source radius (shadow softness control)
	OutSpotLightComponent->SetSoftSourceRadius(InSpotLightItem.soft_source_radius);
	
	// If you want a stretched or elongated source (like a tube), you can give the light some length — useful for stylized or certain lighting setups.
	OutSpotLightComponent->SetSourceLength(InSpotLightItem.source_length);
	
	// When true uses color temperature (Kelvin) to determine its color
	OutSpotLightComponent->SetUseTemperature(InSpotLightItem.use_temperature);
	
	// Set the temperature e.g. 2000K-warm orange, 6500K - neutral white, 10000K - blue tinted cold
	OutSpotLightComponent->SetTemperature(InSpotLightItem.temperature);
	
	// Determines whether the light actually contributes lighting in the world e.g. illuminates surfaces, affects characters, shows up in reflections etc
	OutSpotLightComponent->bAffectsWorld = InSpotLightItem.affects_world;
	
	// Control whether static or dynamic shadows are cast (depending on mobility).
	OutSpotLightComponent->CastShadows = InSpotLightItem.cast_shadows ? 1 : 0;

	// How much light contributes to indirect (bounce) lighting, not direct light (part of GI)
	OutSpotLightComponent->SetIndirectLightingIntensity(InSpotLightItem.indirect_light_intensity);
	
	// Controls how strongly the light interacts with volumetric effects such as fog, mist, dust, smoke, god rays etc
	OutSpotLightComponent->SetVolumetricScatteringIntensity(InSpotLightItem.volumetric_scatter_intensity);
	
	OutSpotLightComponent->MarkRenderStateDirty();
}

/**
 * Serialize the spot light component to the spot light item
 * @param InSpotLightComponent - Spot light component
 * @param OutSpotLightItem - Spot light item
 */
void UVLightAgent::SerializeSpotLightData(const USpotLightComponent* InSpotLightComponent, FVMSpotLightComponent& OutSpotLightItem)
{
	OutSpotLightItem.intensity = InSpotLightComponent->Intensity;
	
	if (InSpotLightComponent->GetLightUnits() == ELightUnits::Candelas)
		OutSpotLightItem.units = "Candelas";
	else if (InSpotLightComponent->GetLightUnits() == ELightUnits::Lumens)
		OutSpotLightItem.units = "Lumens";
	else
		OutSpotLightItem.units = "Unitless";
	
	OutSpotLightItem.color = InSpotLightComponent->LightColor;
	OutSpotLightItem.attenuation_radius = InSpotLightComponent->AttenuationRadius;
	OutSpotLightItem.inner_cone_angle = InSpotLightComponent->InnerConeAngle;
	OutSpotLightItem.outer_cone_angle = InSpotLightComponent->OuterConeAngle;
	OutSpotLightItem.source_radius = InSpotLightComponent->SourceRadius;
	OutSpotLightItem.soft_source_radius = InSpotLightComponent->SoftSourceRadius;
	OutSpotLightItem.source_length = InSpotLightComponent->SourceLength;
	OutSpotLightItem.use_temperature = InSpotLightComponent->bUseTemperature;
	OutSpotLightItem.temperature = InSpotLightComponent->Temperature;
	OutSpotLightItem.affects_world = InSpotLightComponent->bAffectsWorld;
	OutSpotLightItem.cast_shadows = InSpotLightComponent->CastShadows;
	OutSpotLightItem.indirect_light_intensity = InSpotLightComponent->IndirectLightingIntensity;
	OutSpotLightItem.volumetric_scatter_intensity = InSpotLightComponent->VolumetricScatteringIntensity;
}


