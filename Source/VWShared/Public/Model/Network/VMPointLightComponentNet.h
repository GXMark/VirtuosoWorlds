#pragma once

#include "CoreMinimal.h"
#include "VMPointLightComponentNet.generated.h"

USTRUCT()
struct VWSHARED_API FVMPointLightComponentNet
{
	GENERATED_BODY()

	UPROPERTY()
	float intensity;
	UPROPERTY()
	FName units;
	UPROPERTY()
	FColor color;
	UPROPERTY()
	float attenuation_radius;
	UPROPERTY()
	float source_radius;
	UPROPERTY()
	float soft_source_radius;
	UPROPERTY()
	float source_length;
	UPROPERTY()
	bool use_temperature;
	UPROPERTY()
	float temperature;
	UPROPERTY()
	bool affects_world;
	UPROPERTY()
	bool cast_shadows;
	UPROPERTY()
	float indirect_light_intensity;
	UPROPERTY()
	float volumetric_scatter_intensity;

	FVMPointLightComponentNet()
		: intensity(0.f)
		  , units(NAME_None)
		  , color(FColor::Black)
		  , attenuation_radius(0.f)
		  , source_radius(0.f)
		  , soft_source_radius(0.f)
		  , source_length(0.f)
		  , use_temperature(false)
		  , temperature(6500.f)
		  , affects_world(false)
		  , cast_shadows(false)
		  , indirect_light_intensity(0.f)
		  , volumetric_scatter_intensity(0.f)
	{
	}

	FVMPointLightComponentNet(
		const float& Intensity,
		const FName& Units,
		const FColor& Color,
		const float& AttenuationRadius,
		const float& SourceRadius,
		const float& SoftSourceRadius,
		const float& SourceLength,
		const bool& UseTemperature,
		const float& Temperature,
		const bool& AffectsWorld,
		const bool& CastShadows,
		const float& IndirectLightIntensity,
		const float& VolumetricScatterIntensity)
		: intensity(Intensity)
		  , units(Units)
		  , color(Color)
		  , attenuation_radius(AttenuationRadius)
		  , source_radius(SourceRadius)
		  , soft_source_radius(SoftSourceRadius)
		  , source_length(SourceLength)
		  , use_temperature(UseTemperature)
		  , temperature(Temperature)
		  , affects_world(AffectsWorld)
		  , cast_shadows(CastShadows)
		  , indirect_light_intensity(IndirectLightIntensity)
		  , volumetric_scatter_intensity(VolumetricScatterIntensity)
	{
	}

private:
	// ---- 16-bit quantization helpers ----
	static FORCEINLINE uint16 Quantize16(const float V, const float Min, const float Max)
	{
		if (Max <= Min) return 0;
		const float Clamped = FMath::Clamp(V, Min, Max);
		const float Alpha = (Clamped - Min) / (Max - Min); // 0..1
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
		Ar << units;

		// FColor is already compact (RGBA8)
		Ar << color;

		// Pack bools into 3 bits
		uint8 Flags = 0;
		if (Ar.IsSaving())
		{
			Flags |= (use_temperature ? 1 << 0 : 0);
			Flags |= (affects_world ? 1 << 1 : 0);
			Flags |= (cast_shadows ? 1 << 2 : 0);
		}
		Ar.SerializeBits(&Flags, 3);

		if (Ar.IsLoading())
		{
			use_temperature = (Flags & (1 << 0)) != 0;
			affects_world = (Flags & (1 << 1)) != 0;
			cast_shadows = (Flags & (1 << 2)) != 0;
		}

		// ---- Quantized floats ----
		// Ranges (tune as needed):
		// intensity: 0..200000
		// attenuation radius: 0..50000 cm (500m)
		// source radii/length: 0..5000 cm (50m)
		// temperature: 1000..20000 K (only if use_temperature)
		// indirect/volumetric: 0..8

		uint16 QIntensity = 0;
		if (Ar.IsSaving()) QIntensity = Quantize16(intensity, 0.f, 200000.f);
		Ar << QIntensity;
		if (Ar.IsLoading()) intensity = Dequantize16(QIntensity, 0.f, 200000.f);

		uint16 QAtten = 0;
		if (Ar.IsSaving()) QAtten = Quantize16(attenuation_radius, 0.f, 50000.f);
		Ar << QAtten;
		if (Ar.IsLoading()) attenuation_radius = Dequantize16(QAtten, 0.f, 50000.f);

		uint16 QSourceR = 0, QSoftR = 0, QSourceL = 0;
		if (Ar.IsSaving())
		{
			QSourceR = Quantize16(source_radius, 0.f, 5000.f);
			QSoftR = Quantize16(soft_source_radius, 0.f, 5000.f);
			QSourceL = Quantize16(source_length, 0.f, 5000.f);
		}
		Ar << QSourceR;
		Ar << QSoftR;
		Ar << QSourceL;
		if (Ar.IsLoading())
		{
			source_radius = Dequantize16(QSourceR, 0.f, 5000.f);
			soft_source_radius = Dequantize16(QSoftR, 0.f, 5000.f);
			source_length = Dequantize16(QSourceL, 0.f, 5000.f);
		}

		// temperature only if used
		if (use_temperature)
		{
			uint16 QTemp = 0;
			if (Ar.IsSaving()) QTemp = Quantize16(temperature, 1000.f, 20000.f);
			Ar << QTemp;
			if (Ar.IsLoading()) temperature = Dequantize16(QTemp, 1000.f, 20000.f);
		}
		else if (Ar.IsLoading())
		{
			temperature = 6500.f;
		}

		uint16 QIndirect = 0, QVol = 0;
		if (Ar.IsSaving())
		{
			QIndirect = Quantize16(indirect_light_intensity, 0.f, 8.f);
			QVol = Quantize16(volumetric_scatter_intensity, 0.f, 8.f);
		}
		Ar << QIndirect;
		Ar << QVol;
		if (Ar.IsLoading())
		{
			indirect_light_intensity = Dequantize16(QIndirect, 0.f, 8.f);
			volumetric_scatter_intensity = Dequantize16(QVol, 0.f, 8.f);
		}

		return bOutSuccess;
	}

	// ---- Identical (match quantization tolerance + temperature gating) ----
	bool Identical(const FVMPointLightComponentNet* Other, uint32 PortFlags) const
	{
		if (!Other) return false;

		auto Tol16 = [](float Range) { return (Range / 65535.f) * 2.f; };

		const bool bTempSame =
			(use_temperature == Other->use_temperature) &&
			(!use_temperature || (FMath::Abs(temperature - Other->temperature) <= Tol16(19000.f))); // 1000..20000

		return units == Other->units
			&& color == Other->color
			&& affects_world == Other->affects_world
			&& cast_shadows == Other->cast_shadows
			&& bTempSame

			&& FMath::Abs(intensity - Other->intensity) <= Tol16(200000.f)
			&& FMath::Abs(attenuation_radius - Other->attenuation_radius) <= Tol16(50000.f)
			&& FMath::Abs(source_radius - Other->source_radius) <= Tol16(5000.f)
			&& FMath::Abs(soft_source_radius - Other->soft_source_radius) <= Tol16(5000.f)
			&& FMath::Abs(source_length - Other->source_length) <= Tol16(5000.f)
			&& FMath::Abs(indirect_light_intensity - Other->indirect_light_intensity) <= Tol16(8.f)
			&& FMath::Abs(volumetric_scatter_intensity - Other->volumetric_scatter_intensity) <= Tol16(8.f);
	}
};

// Enable NetSerializer for FMPointLightComponentNet
template <>
struct TStructOpsTypeTraits<FVMPointLightComponentNet> : public TStructOpsTypeTraitsBase2<FVMPointLightComponentNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};
