#pragma once

#include "CoreMinimal.h"
#include "VMRotationNet.h"
#include "UObject/NoExportTypes.h"
#include "Engine/NetSerialization.h"
#include "VMTransformNet.generated.h"

USTRUCT()
struct VWSHARED_API FVMTransformNet
{
	GENERATED_BODY()

	UPROPERTY()
	FVector_NetQuantize10 Location;

	UPROPERTY()
	FVMRotationNet Rotation;

	UPROPERTY()
	bool bHasScale = false;

	UPROPERTY()
	FVector_NetQuantize10 Scale = FVector_NetQuantize10(FVector(1.f, 1.f, 1.f));

	// ---- NetSerialize ----
	FORCEINLINE bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// FVector_NetQuantize10 already has NetSerialize via the property system
		Ar << Location;

		// Rotation (your optimized struct)
		{
			bool bOk = true;
			Rotation.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// Pack bHasScale as 1 bit
		uint8 Flags = 0;
		if (Ar.IsSaving())
		{
			Flags |= (bHasScale ? 1 << 0 : 0);
		}
		Ar.SerializeBits(&Flags, 1);

		if (Ar.IsLoading())
		{
			bHasScale = (Flags & (1 << 0)) != 0;
		}

		// Only serialize scale when present
		if (bHasScale)
		{
			Ar << Scale;
		}
		else if (Ar.IsLoading())
		{
			Scale = FVector_NetQuantize10(FVector(1.f, 1.f, 1.f));
		}

		return bOutSuccess;
	}
};

template<>
struct TStructOpsTypeTraits<FVMTransformNet> : public TStructOpsTypeTraitsBase2<FVMTransformNet>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

USTRUCT()
struct VWSHARED_API FHotTransformQ
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid ItemID;

	UPROPERTY()
	FVMTransformNet Xf;

	UPROPERTY()
	bool bInterpolate = false;

	// ---- NetSerialize ----
	FORCEINLINE bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// Serialize ItemID directly (compact)
		Ar << ItemID;

		// Serialize interpolate flag as 1 bit
		uint8 Flags = 0;
		if (Ar.IsSaving())
		{
			Flags |= (bInterpolate ? 1 << 0 : 0);
		}
		Ar.SerializeBits(&Flags, 1);
		if (Ar.IsLoading())
		{
			bInterpolate = (Flags & (1 << 0)) != 0;
		}

		// Transform
		{
			bool bOk = true;
			Xf.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		return bOutSuccess;
	}
};

template<>
struct TStructOpsTypeTraits<FHotTransformQ> : public TStructOpsTypeTraitsBase2<FHotTransformQ>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

// ---- Helpers ----
FORCEINLINE FTransform ToTransform(const FHotTransformQ& Hot)
{
	FTransform Xf;
	Xf.SetLocation(FVector(Hot.Xf.Location));
	Xf.SetRotation(Hot.Xf.Rotation.ToQuat());
	Xf.SetScale3D(Hot.Xf.bHasScale ? FVector(Hot.Xf.Scale) : FVector(1.f, 1.f, 1.f));
	return Xf;
}

FORCEINLINE FHotTransformQ MakeHotQ(const FGuid& ItemID, const FTransform& InXf, bool bInterpolate)
{
	FHotTransformQ Out;
	Out.ItemID = ItemID;
	Out.bInterpolate = bInterpolate;

	Out.Xf.Location = FVector_NetQuantize10(InXf.GetLocation());

	// rotation: normalize first, then quantize in NetSerialize
	Out.Xf.Rotation = FVMRotationNet(InXf.Rotator().GetNormalized());

	const FVector S = InXf.GetScale3D();
	const bool bScaleNotIdentity = !S.Equals(FVector(1.f, 1.f, 1.f), 0.001f);

	Out.Xf.bHasScale = bScaleNotIdentity;
	Out.Xf.Scale = bScaleNotIdentity
		? FVector_NetQuantize10(S)
		: FVector_NetQuantize10(FVector(1.f, 1.f, 1.f));

	return Out;
}