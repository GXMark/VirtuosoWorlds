#pragma once

#include "CoreMinimal.h"
#include "VMGuidNet.generated.h"

/*
 * FMGuidNet
 * Deterministic, bit-packed GUID wrapper for network payloads.
 *
 * Wire format: exactly 128 bits (4 × 32)
 * Safe for RPCs, FastArrays, and hot-lane streaming.
 */
USTRUCT(BlueprintType)
struct VWSHARED_API FVMGuidNet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid Value;

	FVMGuidNet()
		: Value(0, 0, 0, 0)
	{}

	explicit FVMGuidNet(const FGuid& InGuid)
		: Value(InGuid)
	{}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		uint32 A = static_cast<uint32>(Value.A);
		uint32 B = static_cast<uint32>(Value.B);
		uint32 C = static_cast<uint32>(Value.C);
		uint32 D = static_cast<uint32>(Value.D);

		Ar.SerializeBits(&A, 32);
		Ar.SerializeBits(&B, 32);
		Ar.SerializeBits(&C, 32);
		Ar.SerializeBits(&D, 32);

		if (Ar.IsLoading())
		{
			Value.A = static_cast<int32>(A);
			Value.B = static_cast<int32>(B);
			Value.C = static_cast<int32>(C);
			Value.D = static_cast<int32>(D);
		}

		return true;
	}

	// ---- Identical ----
	bool Identical(const FVMGuidNet* Other, uint32 /*PortFlags*/) const
	{
		return Other && Value == Other->Value;
	}
};

template<>
struct TStructOpsTypeTraits<FVMGuidNet>
	: public TStructOpsTypeTraitsBase2<FVMGuidNet>
{
	enum
	{
		WithNetSerializer   = true,
		WithIdentical       = true,
		WithZeroConstructor = true,
	};
};