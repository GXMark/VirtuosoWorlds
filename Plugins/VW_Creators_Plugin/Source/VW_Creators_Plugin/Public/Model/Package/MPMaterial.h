#pragma once

#include "CoreMinimal.h"
#include "MPScalar.h"
#include "MPTexture.h"
#include "MPMaterial.generated.h"

USTRUCT()
struct FMPMaterial
{
	GENERATED_BODY()

	// Material guid identifier
	UPROPERTY()
	FGuid id;
	
	// Material property name
	UPROPERTY()
	FName name;

	// (Master) Material parent name
	UPROPERTY()
	FName parent;

	// Material scalar (float) values
	UPROPERTY()
	TArray<FMPScalar> scalars;

	// Material vectors
	UPROPERTY()
	TArray<FMPLinearColor> vectors;

	// Material texture info
	UPROPERTY()
	TArray<FMPTexture> textures;

	FMPMaterial() : 
		id(FGuid(0,0,0,0))
		, name(NAME_None)
		, parent(NAME_None)
		, scalars({})
		, vectors({})
		, textures({}) 
	{}

	FMPMaterial(const FGuid& InID, const FName& InName, const FName& InParent, const TArray<FMPScalar>& InScalars, const TArray<FMPLinearColor>& InVectors, const TArray<FMPTexture>& InTextures):
		id(InID)
		, name(InName)
		, parent(InParent)
		, scalars(InScalars)
		, vectors(InVectors)
		, textures(InTextures)
	{}
};
