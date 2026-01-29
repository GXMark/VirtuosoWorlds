#include "Region/Spawner/VLightSpawner.h"

/*#include "Region/Actor/PointLightActor.h"
#include "Model/Package/MActor.h"
#include "Region/Actor/SpotLightActor.h"

/**
 * Spawn spot light actor
 * @param InActorItem - Actor item
 * @param InWorld - World state
 * @return Spot light actor
 #1#
ASpotLightActor* FLightSpawner::SpawnSpotLightActor(const FMActor& InActorItem, UWorld* InWorld)
{
	if (ASpotLightActor* LightActor = InWorld->SpawnActor<ASpotLightActor>(ASpotLightActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator))
	{
		LightActor->Component->SetMobility(EComponentMobility::Movable);

		// Move the actor to the desired location and rotation
		FVector Location(FVector(InActorItem.transform.position));
		FRotator Rotation(InActorItem.transform.rotation.Y, InActorItem.transform.rotation.Z, InActorItem.transform.rotation.X);
		LightActor->SetActorLocation(Location);
		LightActor->SetActorRotation(Rotation);

		/#1#/ Set properties
		SpotLightComp->SetIntensity(Component.spot_light_comp.intensity);

		FName Units = Component.spot_light_comp.units;
		if (Units == FLightUnitType::Candelas)
			SpotLightComp->SetIntensityUnits(ELightUnits::Candelas);
		else if (Units == FLightUnitType::Lumens)
			SpotLightComp->SetIntensityUnits(ELightUnits::Lumens);
		else if (Units == FLightUnitType::EV)
			SpotLightComp->SetIntensityUnits(ELightUnits::EV);
		else
			SpotLightComp->SetIntensityUnits(ELightUnits::Unitless);

		SpotLightComp->SetLightColor(Component.spot_light_comp.color);

		SpotLightComp->SetAttenuationRadius(Component.spot_light_comp.attenuation_radius);
		SpotLightComp->SetInnerConeAngle(Component.spot_light_comp.inner_cone_angle);
		SpotLightComp->SetOuterConeAngle(Component.spot_light_comp.outer_cone_angle);
		SpotLightComp->SetSourceRadius(Component.spot_light_comp.source_radius);
		SpotLightComp->SetSourceLength(Component.spot_light_comp.source_length);
		SpotLightComp->SetSoftSourceRadius(Component.spot_light_comp.soft_source_radius);
		SpotLightComp->SetUseTemperature(Component.spot_light_comp.use_temperature);
		SpotLightComp->SetTemperature(Component.spot_light_comp.temperature);
		SpotLightComp->bAffectsWorld = Component.spot_light_comp.affects_world;
		SpotLightComp->CastShadows = Component.spot_light_comp.cast_shadows == 1;
		SpotLightComp->SetIndirectLightingIntensity(Component.spot_light_comp.indirect_light_intensity);
		SpotLightComp->SetVolumetricScatteringIntensity(Component.spot_light_comp.volumetric_scatter_intensity);#1#

		//UE_LOG(LogTemp, Log, TEXT("Light Stream Helper - Spawn Spot Light Actor [ Status: Success, Position: %s, Rotation: %s ]"),
		//       *Component.transform.position.ToString(), *Component.transform.rotation.ToString());

		// Wake up for replication
		LightActor->FlushNetDormancy();
		
		return LightActor;
	}

	UE_LOG(LogTemp, Error, TEXT("Light Stream Helper - Spawn Spot Light Actor [ Status: Actor not created ]"));

	return nullptr;
}


//Spawn point light actor

APointLightActor* FLightSpawner::SpawnPointLightActor(const FMActor& InActorItem, UWorld* InWorld)
{
	if (APointLightActor* LightActor = InWorld->SpawnActor<APointLightActor>(APointLightActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator))
	{
		LightActor->Component->SetMobility(EComponentMobility::Movable);

		// Move the actor to the desired location and rotation
		FVector Location(FVector(InActorItem.transform.position));
		FRotator Rotation(InActorItem.transform.rotation.Y, InActorItem.transform.rotation.Z, InActorItem.transform.rotation.X);
		LightActor->SetActorLocation(Location);
		LightActor->SetActorRotation(Rotation);
		
/*
		// Set properties

		PointLightComp->SetIntensity(Component.point_light_comp.intensity);

		FName Units = Component.point_light_comp.units;
		if (Units == FLightUnitType::Candelas)
			PointLightComp->SetIntensityUnits(ELightUnits::Candelas);
		else if (Units == FLightUnitType::Lumens)
			PointLightComp->SetIntensityUnits(ELightUnits::Lumens);
		else if (Units == FLightUnitType::EV)
			PointLightComp->SetIntensityUnits(ELightUnits::EV);
		else
			PointLightComp->SetIntensityUnits(ELightUnits::Unitless);

		PointLightComp->SetLightColor(Component.point_light_comp.color);
		PointLightComp->SetAttenuationRadius(Component.point_light_comp.attenuation_radius);
		PointLightComp->SetSourceRadius(Component.point_light_comp.source_radius);
		PointLightComp->SetSoftSourceRadius(Component.point_light_comp.soft_source_radius);
		PointLightComp->SetSourceLength(Component.point_light_comp.source_length);
		PointLightComp->SetUseTemperature(Component.point_light_comp.use_temperature);
		PointLightComp->SetTemperature(Component.point_light_comp.temperature);
		PointLightComp->bAffectsWorld = Component.point_light_comp.affects_world;
		PointLightComp->CastShadows = Component.point_light_comp.cast_shadows == 1;
		PointLightComp->SetIndirectLightingIntensity(Component.point_light_comp.indirect_light_intensity);
		PointLightComp->SetVolumetricScatteringIntensity(Component.point_light_comp.volumetric_scatter_intensity);
		#1#

		//UE_LOG(LogTemp, Log, TEXT("Light Stream Helper - Spawn Point Light Actor [ Status: Success, Position: %s, Rotation: %s ]"),
		//       *Component.transform.position.ToString(), *Component.transform.rotation.ToString());

		// Wake up for replication
		LightActor->FlushNetDormancy();
		
		return LightActor;
	}


	UE_LOG(LogTemp, Error, TEXT("Light Stream Helper - Spawn Point Light Actor [ Status: Actor not created ]"));

	return nullptr;
}*/
