#pragma once

#include "CoreMinimal.h"
#include "VMScalar.h"
#include "VMLinearColor.h"
#include "VMTexture.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "VMMaterial.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FVMMaterial : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// Material guid identifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FGuid id;

	// Material property name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName name;

	// (Master) Material parent name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName parent;

	// Material scalar (float) values
	UPROPERTY(EditAnywhere, Category="Materials", meta=(ShowOnlyInnerProperties))
	TArray<FVMScalar> scalars;

	// Material vectors
	UPROPERTY(EditAnywhere, Category="Materials", meta=(ShowOnlyInnerProperties))
	TArray<FVMLinearColor> vectors;

	// Material texture info
	UPROPERTY(EditAnywhere, Category="Materials", meta=(ShowOnlyInnerProperties))
	TArray<FVMTexture> textures;

	FVMMaterial() 
		: id(FGuid(0, 0, 0, 0))
		, name(NAME_None)
		, parent(NAME_None)
		, scalars({})
		, vectors({})
		, textures({})
	{}

	FVMMaterial(const FGuid& ID, const FString& Name, const FString& Parent, const TArray<FVMScalar>& Scalars, const TArray<FVMLinearColor>& Vectors, const TArray<FVMTexture>& Textures):
		id(ID)
		, name(Name)
		, parent(Parent)
		, scalars(Scalars)
		, vectors(Vectors)
		, textures(Textures)
	{}
};
