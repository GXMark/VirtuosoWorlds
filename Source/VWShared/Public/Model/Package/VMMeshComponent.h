#pragma once

#include "CoreMinimal.h"
#include "VMAssetRef.h"
#include "VMMeshComponent.generated.h"

USTRUCT()
struct VWSHARED_API FVMMeshComponent
{
	GENERATED_BODY()

	UPROPERTY()
	FVMAssetRef mesh_ref;

	UPROPERTY()
	TArray<FVMAssetRef> material_refs;
	
	FVMMeshComponent() :
		mesh_ref({})
		, material_refs({})
	{}

	FVMMeshComponent(const FVMAssetRef& InMeshRef, const TArray<FVMAssetRef>& InMaterialRefs) :
		mesh_ref(InMeshRef)
		, material_refs(InMaterialRefs)
	{}
};