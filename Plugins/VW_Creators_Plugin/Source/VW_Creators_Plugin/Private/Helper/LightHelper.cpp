#include "Helper/LightHelper.h"

#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Constant/Constants.h"
#include "Helper/StatsHelper.h"
#include "Model/Package/MPActor.h"

/**
 * Serialize the point light component
 * @param InComponent - Point light component
 * @param OutActorItem - Actor item
 */
bool FCPLightHelper::SerializePointLightComponent(const UPointLightComponent* InComponent, FMPActor& OutActorItem)
{
	FMPPointLightComponent PLComponent;

	PLComponent.intensity = InComponent->Intensity;

	switch (InComponent->GetLightUnits())
	{
	case ELightUnits::Candelas:
		{
			PLComponent.units = TEXT("Candelas");
			break;
		}
	case ELightUnits::Lumens:
		{
			PLComponent.units = TEXT("Lumens");
			break;
		}
	default:
		{
			PLComponent.units = TEXT("Unitless");
			break;
		}
	}

	PLComponent.color = InComponent->LightColor;
	PLComponent.attenuation_radius = InComponent->AttenuationRadius;
	PLComponent.source_radius = InComponent->SourceRadius;
	PLComponent.soft_source_radius = InComponent->SoftSourceRadius;
	PLComponent.source_length = InComponent->SourceLength;
	PLComponent.use_temperature = InComponent->bUseTemperature;
	PLComponent.temperature = InComponent->Temperature;
	PLComponent.affects_world = InComponent->bAffectsWorld;
	PLComponent.cast_shadows = InComponent->CastShadows == 1;
	PLComponent.indirect_light_intensity = InComponent->IndirectLightingIntensity;
	PLComponent.volumetric_scatter_intensity = InComponent->VolumetricScatteringIntensity;

	OutActorItem.point_light_comp = PLComponent;
	
	FCPStatsHelper::TotalPointLights++;

	return true;
}

/**
 * Serialize the spot-light component
 * @param InComponent - Spot-light component
 * @param OutActorItem - Actor item
 * @return Success - true, Fail - false
 */
bool FCPLightHelper::SerializeSpotLightComponent(const USpotLightComponent* InComponent, FMPActor& OutActorItem)
{
	FMPSpotLightComponent SLComponent;

	SLComponent.intensity = InComponent->Intensity;

	switch (InComponent->GetLightUnits())
	{
	case ELightUnits::Candelas:
		{
			SLComponent.units = FPLightUnitType::Candelas;
			break;
		}
	case ELightUnits::Lumens:
		{
			SLComponent.units = FPLightUnitType::Lumens;
			break;
		}
	case ELightUnits::EV:
		{
			SLComponent.units = FPLightUnitType::EV;
			break;
		}
	default:
		{
			SLComponent.units = FPLightUnitType::Unitless;
			break;
		}
	}

	SLComponent.color = InComponent->LightColor;
	SLComponent.attenuation_radius = InComponent->AttenuationRadius;
	SLComponent.source_radius = InComponent->SourceRadius;
	SLComponent.inner_cone_angle = InComponent->InnerConeAngle;
	SLComponent.outer_cone_angle = InComponent->OuterConeAngle;
	SLComponent.soft_source_radius = InComponent->SoftSourceRadius;
	SLComponent.source_length = InComponent->SourceLength;
	SLComponent.use_temperature = InComponent->bUseTemperature;
	SLComponent.temperature = InComponent->Temperature;
	SLComponent.affects_world = InComponent->bAffectsWorld;
	SLComponent.cast_shadows = InComponent->CastShadows == 1;
	SLComponent.indirect_light_intensity = InComponent->IndirectLightingIntensity;
	SLComponent.volumetric_scatter_intensity = InComponent->VolumetricScatteringIntensity;

	OutActorItem.spot_light_comp = SLComponent;
	
	FCPStatsHelper::TotalSpotLights++;

	return true;
}