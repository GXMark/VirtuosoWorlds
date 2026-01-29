#pragma once

#include "CoreMinimal.h"
#include "MPMeshComponent.generated.h"

USTRUCT()
struct FMPMeshComponent
{
	GENERATED_BODY()

	UPROPERTY()
	FMPAssetRef mesh_ref;

	UPROPERTY()
	TArray<FMPAssetRef> material_refs;
	
	FMPMeshComponent():
		mesh_ref({})
		, material_refs({})
	{}
	
	FMPMeshComponent(const FMPAssetRef& InMeshRef, const TArray<FMPAssetRef>& InMaterialRefs):
		mesh_ref(InMeshRef)
		, material_refs(InMaterialRefs)
	{}
};
