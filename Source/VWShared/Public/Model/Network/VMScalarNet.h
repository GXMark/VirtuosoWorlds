#pragma once

#include "CoreMinimal.h"
#include "VMScalarNet.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FVMScalarNet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	float value;

	FVMScalarNet()
		: name(NAME_None)
		, value(0.f)
	{}

	FVMScalarNet(const FName& Name, const float Value)
		: name(Name)
		, value(Value)
	{}

private:
	// Quantization range for this scalar type
	static constexpr float MinV = -64.f;
	static constexpr float MaxV =  64.f;

	// Generic 16-bit quantize/dequantize
	static FORCEINLINE uint16 Quantize16(const float V, const float Min, const float Max)
	{
		if (Max <= Min) return 0;
		const float Clamped = FMath::Clamp(V, Min, Max);
		const float Alpha = (Clamped - Min) / (Max - Min);
		const int32 Q = FMath::RoundToInt(Alpha * 65535.0f);
		return static_cast<uint16>(FMath::Clamp(Q, 0, 65535));
	}

	static FORCEINLINE float Dequantize16(const uint16 Q, const float Min, const float Max)
	{
		if (Max <= Min) return Min;
		const float Alpha = static_cast<float>(Q) / 65535.0f;
		return Min + Alpha * (Max - Min);
	}

public:
	// ---- NetSerialize (optimized) ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// FName is net-deduped per connection
		Ar << name;

		uint16 Q = 0;
		if (Ar.IsSaving()) Q = Quantize16(value, MinV, MaxV);
		Ar << Q;
		if (Ar.IsLoading()) value = Dequantize16(Q, MinV, MaxV);

		return bOutSuccess;
	}

	// ---- Identical (match quantization tolerance) ----
	bool Identical(const FVMScalarNet* Other, uint32 PortFlags) const
	{
		if (!Other) return false;

		const float Range = MaxV - MinV;           // 128
		const float Tol   = (Range / 65535.f) * 2.f; // ~2 LSBs

		return name == Other->name
			&& FMath::Abs(value - Other->value) <= Tol;
	}
};

template<>
struct TStructOpsTypeTraits<FVMScalarNet> : public TStructOpsTypeTraitsBase2<FVMScalarNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};
