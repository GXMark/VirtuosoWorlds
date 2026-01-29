#pragma once

#include "CoreMinimal.h"
#include "VMRotationNet.generated.h"

/*
 * MMO Optimized Rotation (48 bits / 6 bytes)
 * Stores Pitch, Roll, Yaw as signed 16-bit "angle short" with wrapping.
 *
 * - No clamping (prevents incorrect snaps at >180°)
 * - Explicit bit packing (always 48 bits in NetSerialize)
 */
USTRUCT()
struct FVMRotationNet
{
	GENERATED_BODY()

	// Stored as signed 16-bit angles.
	// Degrees in [-180, +180) mapped to int16 in [-32768, +32767]
	UPROPERTY()
	int16 PitchS = 0;

	UPROPERTY()
	int16 RollS = 0;

	UPROPERTY()
	int16 YawS = 0;

	FVMRotationNet() = default;

	FVMRotationNet(int16 InPitchS, int16 InRollS, int16 InYawS)
		: PitchS(InPitchS), RollS(InRollS), YawS(InYawS)
	{
	}

	explicit FVMRotationNet(const FRotator& R)
	{
		*this = FromRotator(R);
	}

	explicit FVMRotationNet(const FQuat& Q)
	{
		*this = FromRotator(Q.Rotator());
	}

	// ---- Quantization helpers ----

	// Wrap degrees into [-180, +180)
	static FORCEINLINE float WrapDeg180(float Deg)
	{
		// UnwindDegrees -> (-180, 180] typically; we then shift 180 to -180 for stable packing.
		float U = FMath::UnwindDegrees(Deg);
		if (U >= 180.f)
		{
			U -= 360.f;
		}
		return U;
	}

	static FORCEINLINE int16 DegToShort(float Deg)
	{
		// Map [-180, +180) -> [-32768, +32767]
		// Use rounding to nearest to reduce bias.
		const float W = WrapDeg180(Deg);
		const float Scaled = (W / 180.f) * 32768.f;

		// Clamp to int16 range in case of tiny numerical overshoot.
		const int32 I = FMath::Clamp(FMath::RoundToInt(Scaled), -32768, 32767);
		return static_cast<int16>(I);
	}

	static FORCEINLINE float ShortToDeg(int16 S)
	{
		// Inverse mapping back to degrees.
		return (static_cast<float>(S) * 180.f) / 32768.f;
	}

	// ---- Construction ----

	static FORCEINLINE FVMRotationNet FromRotator(const FRotator& R)
	{
		// Normalize to stable canonical form before packing.
		const FRotator N = R.GetNormalized();

		// Store explicitly as Pitch/Roll/Yaw (no axis confusion)
		return FVMRotationNet(
			DegToShort(N.Pitch),
			DegToShort(N.Roll),
			DegToShort(N.Yaw)
		);
	}

	static FORCEINLINE FVMRotationNet FromQuat(const FQuat& Q)
	{
		return FromRotator(Q.Rotator());
	}

	// ---- Conversion ----

	FORCEINLINE FRotator ToRotator() const
	{
		// Return in UE's standard Pitch/Yaw/Roll order
		return FRotator(
			ShortToDeg(PitchS),
			ShortToDeg(YawS),
			ShortToDeg(RollS)
		);
	}

	FORCEINLINE FQuat ToQuat() const
	{
		return ToRotator().Quaternion();
	}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// Explicitly pack exactly 16 bits per component (48 bits total).
		// SerializeBits expects a pointer to a buffer; easiest is to use uint16 temps.
		uint16 P = static_cast<uint16>(PitchS);
		uint16 R = static_cast<uint16>(RollS);
		uint16 Y = static_cast<uint16>(YawS);

		Ar.SerializeBits(&P, 16);
		Ar.SerializeBits(&R, 16);
		Ar.SerializeBits(&Y, 16);

		if (Ar.IsLoading())
		{
			PitchS = static_cast<int16>(P);
			RollS  = static_cast<int16>(R);
			YawS   = static_cast<int16>(Y);
		}

		return true;
	}

	// ---- Identical / comparisons ----

	bool Identical(const FVMRotationNet* Other, uint32 /*PortFlags*/) const
	{
		return Other
			&& PitchS == Other->PitchS
			&& RollS  == Other->RollS
			&& YawS   == Other->YawS;
	}

	FORCEINLINE bool IsNearlyEqual(const FVMRotationNet& Other) const
	{
		// Exact equality is typically what you want for packed network values.
		return PitchS == Other.PitchS && RollS == Other.RollS && YawS == Other.YawS;
	}
};

// Enable UE NetSerializer for FMRotationNet
template<>
struct TStructOpsTypeTraits<FVMRotationNet> : public TStructOpsTypeTraitsBase2<FVMRotationNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
	};
};