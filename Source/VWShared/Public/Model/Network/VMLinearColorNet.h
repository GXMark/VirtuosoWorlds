#pragma once

#include "CoreMinimal.h"
#include "VMLinearColorNet.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FVMLinearColorNet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName name;

	// Stored as 8-bit per channel RGBA (0..255)
	// This is ~4 bytes on the wire vs 16 bytes (4 floats) previously.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FLinearColor color;

	FVMLinearColorNet()
		: name(NAME_None)
		, color(0, 0, 0, 0)
	{}

	FVMLinearColorNet(const FName& Name, const FLinearColor& Color)
		: name(Name)
		, color(Color)
	{}

	explicit FVMLinearColorNet(const FLinearColor& Color)
		: name(NAME_None)
		, color(Color)
	{}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		Ar << name;

		// Quantize to 8-bit per channel.
		// NOTE: FLinearColor is usually linear; converting to FColor assumes 0..1 range.
		// If you use HDR values >1, clamp or add a scale/exposure scheme.
		uint32 PackedRGBA = 0;

		if (Ar.IsSaving())
		{
			const FColor C = color.ToFColor(true); // true = sRGB conversion; set false if you want raw linear clamp
			PackedRGBA =
				(static_cast<uint32>(C.R) << 24) |
				(static_cast<uint32>(C.G) << 16) |
				(static_cast<uint32>(C.B) <<  8) |
				(static_cast<uint32>(C.A) <<  0);
		}

		Ar.SerializeBits(&PackedRGBA, 32);

		if (Ar.IsLoading())
		{
			FColor C;
			C.R = static_cast<uint8>((PackedRGBA >> 24) & 0xFF);
			C.G = static_cast<uint8>((PackedRGBA >> 16) & 0xFF);
			C.B = static_cast<uint8>((PackedRGBA >>  8) & 0xFF);
			C.A = static_cast<uint8>((PackedRGBA >>  0) & 0xFF);

			// Convert back to linear color
			color = C.ReinterpretAsLinear(); // if you used ToFColor(false), consider FLinearColor(C)
		}

		return true;
	}

	// ---- Identical ----
	bool Identical(const FVMLinearColorNet* Other, uint32 /*PortFlags*/) const
	{
		if (!Other) return false;
		if (name != Other->name) return false;

		// Compare in the same quantized domain to avoid float epsilon quirks.
		const FColor A = color.ToFColor(true);
		const FColor B = Other->color.ToFColor(true);
		return A.DWColor() == B.DWColor();
	}
};

template<>
struct TStructOpsTypeTraits<FVMLinearColorNet> : public TStructOpsTypeTraitsBase2<FVMLinearColorNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
	};
};
