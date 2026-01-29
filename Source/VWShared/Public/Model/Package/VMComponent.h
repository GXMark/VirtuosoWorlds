#pragma once

#include "CoreMinimal.h"
#include "VMDecalComponent.h"
#include "VMSpotLightComponent.h"
#include "VMMeshComponent.h"
#include "VMPointLightComponent.h"
#include "VMComponent.generated.h"

USTRUCT()
struct VWSHARED_API FVMComponent
{
	GENERATED_BODY()

	// Shared collision definition id (maps to FVMCollision in the collision table)
	UPROPERTY()
	FGuid collision_id = FGuid(0,0,0,0);
	
	UPROPERTY()
	FVMMeshComponent mesh_comp; // mesh component

	UPROPERTY()
	FVMDecalComponent decal_comp; // decal component

	UPROPERTY()
	FVMPointLightComponent point_light_comp; // point light component

	UPROPERTY()
	FVMSpotLightComponent spot_light_comp; // spot light component

	FVMComponent() {}

	FVMComponent(const FGuid& InCollisionId, const FVMMeshComponent& MeshComp, const FVMDecalComponent& DecalComp, const FVMPointLightComponent& PointLightComp, const FVMSpotLightComponent& SpotLightComp):
		collision_id(InCollisionId)
		, mesh_comp(MeshComp)
		, decal_comp(DecalComp)
		, point_light_comp(PointLightComp)
		, spot_light_comp(SpotLightComp)
	{}
};