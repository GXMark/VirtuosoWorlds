#pragma once

#include "CoreMinimal.h"
#include "VMComponent.h"
#include "VMTransform.h"
#include "VMActor.generated.h"

USTRUCT()
struct VWSHARED_API FVMActor
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
	FVMTransform transform;
	
	UPROPERTY()
	FGuid collision_id;
	
	UPROPERTY()
	FVMMeshComponent mesh_comp;

	UPROPERTY()
	FVMDecalComponent decal_comp;

	UPROPERTY()
	FVMPointLightComponent point_light_comp;

	UPROPERTY()
	FVMSpotLightComponent spot_light_comp;

	FVMActor()	{}
	
	FVMActor(const FGuid& InID, 
			 const FGuid& InPID, 
			 const FName& InName, 
			 const FName& InType, 
			 const FVMTransform& InTransform, 
			 const FGuid& InCollisionID,
			 const FVMMeshComponent& InMeshComp,
			 const FVMDecalComponent& InDecalComp,
			 const FVMPointLightComponent& InPointLightComp,
			 const FVMSpotLightComponent& InSpotLightComp):
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

