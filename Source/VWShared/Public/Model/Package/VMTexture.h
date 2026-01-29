#pragma once

#include "CoreMinimal.h"
#include "Model/Package/VMAssetRef.h"
#include "VMTexture.generated.h"

/*
 * Authored material texture binding.
 *
 *  - asset_ref : canonical identity + optional editor path
 *  - property  : material parameter name to bind the texture to
 *  - linear    : sampling space (linear vs sRGB) used when decoding CRN textures
 */
USTRUCT(BlueprintType)
struct VWSHARED_API FVMTexture
{
	GENERATED_BODY()

	UPROPERTY()
	FVMAssetRef asset_ref;

	UPROPERTY()
	FName property = NAME_None;

	UPROPERTY()
	bool linear = false;

	FVMTexture() = default;

	FVMTexture(const FVMAssetRef& InAssetRef, const FName& InProperty, const bool bInLinear)
		: asset_ref(InAssetRef)
		, property(InProperty)
		, linear(bInLinear)
	{}
};
