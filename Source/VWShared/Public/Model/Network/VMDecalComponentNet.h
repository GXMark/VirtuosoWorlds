#pragma once

#include "CoreMinimal.h"
#include "VMGuidNet.h"
#include "VMLinearColorNet.h"
#include "VMDecalComponentNet.generated.h"

USTRUCT()
struct VWSHARED_API FVMDecalComponentNet
{
	GENERATED_BODY()

	UPROPERTY()
	FVMGuidNet material_id;
	UPROPERTY()
	FVector3d size;
	UPROPERTY()
	FVMLinearColorNet color;
	UPROPERTY()
	float fade_screen_size;
	UPROPERTY()
	float fade_in_start_delay;
	UPROPERTY()
	float fade_in_duration;
	UPROPERTY()
	float fade_out_start_delay;
	UPROPERTY()
	float fade_out_duration;
	UPROPERTY()
	bool destroy_after_fade_out;

	FVMDecalComponentNet()
		: material_id({})
		  , size(0.0, 0.0, 0.0)
		  , color({})
		  , fade_screen_size(0.f)
		  , fade_in_start_delay(0.f)
		  , fade_in_duration(0.f)
		  , fade_out_start_delay(0.f)
		  , fade_out_duration(0.f)
		  , destroy_after_fade_out(false)
	{
	}

	FVMDecalComponentNet(
		const FVMGuidNet& InMaterialId,
		const FVector3d& InSize,
		const FVMLinearColorNet& InColor,
		const float InFadeScreenSize,
		const float InFadeInStartDelay,
		const float InFadeInDuration,
		const float InFadeOutStartDelay,
		const float InFadeOutDuration,
		const bool InDestroyAfterFadeOut)
		: material_id(InMaterialId)
		  , size(InSize)
		  , color(InColor)
		  , fade_screen_size(InFadeScreenSize)
		  , fade_in_start_delay(InFadeInStartDelay)
		  , fade_in_duration(InFadeInDuration)
		  , fade_out_start_delay(InFadeOutStartDelay)
		  , fade_out_duration(InFadeOutDuration)
		  , destroy_after_fade_out(InDestroyAfterFadeOut)
	{
	}

private:
	// ---- 16-bit quantization helpers ----
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

	static FORCEINLINE uint16 QuantizeTime16(const float Seconds)
	{
		// 0..60s is plenty for decal fades (tune if needed)
		return Quantize16(Seconds, 0.f, 60.f);
	}

	static FORCEINLINE float DequantizeTime16(const uint16 Q)
	{
		return Dequantize16(Q, 0.f, 60.f);
	}

public:
	// ---- NetSerialize (optimized) ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// material_id (your optimized guid)
		{
			bool bOk = true;
			material_id.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// Flags (currently only 1 bool, but SerializeBits keeps it tight)
		uint8 Flags = 0;
		if (Ar.IsSaving())
		{
			Flags |= (destroy_after_fade_out ? 1 << 0 : 0);
		}
		Ar.SerializeBits(&Flags, 1);

		if (Ar.IsLoading())
		{
			destroy_after_fade_out = (Flags & (1 << 0)) != 0;
		}

		// size: quantize each component as 16-bit.
		// Range 0..20000 cm (0..200m) for decal extents — tune if you use larger.
		uint16 QSizeX = 0, QSizeY = 0, QSizeZ = 0;
		if (Ar.IsSaving())
		{
			QSizeX = Quantize16(static_cast<float>(size.X), 0.f, 20000.f);
			QSizeY = Quantize16(static_cast<float>(size.Y), 0.f, 20000.f);
			QSizeZ = Quantize16(static_cast<float>(size.Z), 0.f, 20000.f);
		}
		Ar << QSizeX;
		Ar << QSizeY;
		Ar << QSizeZ;
		if (Ar.IsLoading())
		{
			size.X = static_cast<double>(Dequantize16(QSizeX, 0.f, 20000.f));
			size.Y = static_cast<double>(Dequantize16(QSizeY, 0.f, 20000.f));
			size.Z = static_cast<double>(Dequantize16(QSizeZ, 0.f, 20000.f));
		}

		// color (your optimized linear color net)
		{
			bool bOk = true;
			color.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// fade_screen_size: typically small (0..1-ish). Quantize tightly.
		uint16 QFadeScreen = 0;
		if (Ar.IsSaving()) QFadeScreen = Quantize16(fade_screen_size, 0.f, 1.f);
		Ar << QFadeScreen;
		if (Ar.IsLoading()) fade_screen_size = Dequantize16(QFadeScreen, 0.f, 1.f);

		// fade timings: quantize to 16-bit in 0..60 seconds
		uint16 QInStart = 0, QInDur = 0, QOutStart = 0, QOutDur = 0;
		if (Ar.IsSaving())
		{
			QInStart = QuantizeTime16(fade_in_start_delay);
			QInDur = QuantizeTime16(fade_in_duration);
			QOutStart = QuantizeTime16(fade_out_start_delay);
			QOutDur = QuantizeTime16(fade_out_duration);
		}
		Ar << QInStart;
		Ar << QInDur;
		Ar << QOutStart;
		Ar << QOutDur;
		if (Ar.IsLoading())
		{
			fade_in_start_delay = DequantizeTime16(QInStart);
			fade_in_duration = DequantizeTime16(QInDur);
			fade_out_start_delay = DequantizeTime16(QOutStart);
			fade_out_duration = DequantizeTime16(QOutDur);
		}

		return bOutSuccess;
	}

	// ---- Identical (match quantization tolerance) ----
	bool Identical(const FVMDecalComponentNet* Other, uint32 PortFlags) const
	{
		if (!Other) return false;

		auto Tol16 = [](float Range) { return (Range / 65535.f) * 2.f; };

		return material_id.Identical(&Other->material_id, PortFlags)
			&& color.Identical(&Other->color, PortFlags)
			&& destroy_after_fade_out == Other->destroy_after_fade_out

			// size range 0..20000
			&& FMath::Abs(static_cast<float>(size.X - Other->size.X)) <= Tol16(20000.f)
			&& FMath::Abs(static_cast<float>(size.Y - Other->size.Y)) <= Tol16(20000.f)
			&& FMath::Abs(static_cast<float>(size.Z - Other->size.Z)) <= Tol16(20000.f)

			// fade_screen_size 0..1
			&& FMath::Abs(fade_screen_size - Other->fade_screen_size) <= Tol16(1.f)

			// times 0..60
			&& FMath::Abs(fade_in_start_delay - Other->fade_in_start_delay) <= Tol16(60.f)
			&& FMath::Abs(fade_in_duration - Other->fade_in_duration) <= Tol16(60.f)
			&& FMath::Abs(fade_out_start_delay - Other->fade_out_start_delay) <= Tol16(60.f)
			&& FMath::Abs(fade_out_duration - Other->fade_out_duration) <= Tol16(60.f);
	}
};

// Enable NetSerializer for FMDecalComponentNet
template <>
struct TStructOpsTypeTraits<FVMDecalComponentNet> : public TStructOpsTypeTraitsBase2<FVMDecalComponentNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};
