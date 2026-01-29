#pragma once

#include "CoreMinimal.h"
#include "MPDecalComponent.h"
#include "MPMeshComponent.h"
#include "MPPointLightComponent.h"
#include "MPSpotLightComponent.h"
#include "MPTransform.h"
#include "MPActor.generated.h"

USTRUCT()
struct FMPActor
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid id;

	UPROPERTY()
	FGuid pid;

	UPROPERTY()
	FName name;

	UPROPERTY()
	FName type;
    
	UPROPERTY()
	FMPTransform transform;
	
	UPROPERTY()
	FGuid collision_id;
	
	UPROPERTY()
	FMPMeshComponent mesh_comp;

	UPROPERTY()
	FMPDecalComponent decal_comp;

	UPROPERTY()
	FMPPointLightComponent point_light_comp;

	UPROPERTY()
	FMPSpotLightComponent spot_light_comp;

	FMPActor()	{}
	
	FMPActor(const FGuid& InID, 
			 const FGuid& InPID, 
			 const FName& InName, 
			 const FName& InType, 
			 const FTransform& InTransform, 
			 const FGuid& InCollisionID,
			 const FMPMeshComponent& InMeshComp,
			 const FMPDecalComponent& InDecalComp,
			 const FMPPointLightComponent& InPointLightComp,
			 const FMPSpotLightComponent& InSpotLightComp):
		id(InID)
		, pid(InPID)
		, name(InName)
		, type(InType)
		, transform(InTransform)
		, collision_id(InCollisionID)
		, mesh_comp(InMeshComp)
		, decal_comp(InDecalComp)
		, point_light_comp(InPointLightComp)
		, spot_light_comp(InSpotLightComp)
	{}
};
