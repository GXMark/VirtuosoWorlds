#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMAssetRefNet.h"
#include "VMTextureNet.generated.h"

/*
 * Network texture binding for authored materials.
 *
 *  - asset_ref : canonical asset identity and editor path (used for PIE async loads)
 *  - property  : material parameter name to bind the texture to
 *  - linear    : sampling space (linear vs sRGB) used when decoding CRN textures
 */
USTRUCT(BlueprintType)
struct VWSHARED_API FVMTextureNet
{
	GENERATED_BODY()
	
	UPROPERTY()
	FVMAssetRefNet asset_ref;

	UPROPERTY()
	FName property = NAME_None;

	UPROPERTY()
	bool linear = false;

	FVMTextureNet() = default;

	FVMTextureNet(const FVMAssetRefNet& InAssetRef, const FName& InProperty, const bool bInLinear)
		: asset_ref(InAssetRef)
		, property(InProperty)
		, linear(bInLinear)
	{}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// asset_ref (contains id/name/type/path)
		{
			bool bOk = true;
			asset_ref.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// property (FName is net-optimized/deduped per connection)
		Ar << property;

		// 1-bit linear flag (pack tightly)
		uint8 LinearBit = linear ? 1 : 0;
		Ar.SerializeBits(&LinearBit, 1);
		linear = (LinearBit != 0);

		return bOutSuccess;
	}

	// ---- Identical ----
	bool Identical(const FVMTextureNet* Other, uint32 /*PortFlags*/) const
	{
		return Other
			&& asset_ref.Identical(&Other->asset_ref, 0)
			&& property == Other->property
			&& linear == Other->linear;
	}
};

template<>
struct TStructOpsTypeTraits<FVMTextureNet>
	: public TStructOpsTypeTraitsBase2<FVMTextureNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
	};
};
