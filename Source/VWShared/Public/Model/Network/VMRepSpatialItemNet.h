#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMGuidNet.h"
#include "Model/Network/VMTransformNet.h"
#include "VMRepSpatialItemNet.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FSpatialItemId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMGuidNet Value;

	FSpatialItemId()
		: Value()
	{}

	explicit FSpatialItemId(const FGuid& InGuid)
		: Value(InGuid)
	{}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		return Value.NetSerialize(Ar, Map, bOutSuccess);
	}

	bool Identical(const FSpatialItemId* Other, uint32 PortFlags) const
	{
		return Other && Value.Identical(&Other->Value, PortFlags);
	}
};

template<>
struct TStructOpsTypeTraits<FSpatialItemId> : public TStructOpsTypeTraitsBase2<FSpatialItemId>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithZeroConstructor = true,
	};
};

USTRUCT(BlueprintType)
struct VWSHARED_API FMeshAssetId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMGuidNet Value;

	FMeshAssetId()
		: Value()
	{}

	explicit FMeshAssetId(const FGuid& InGuid)
		: Value(InGuid)
	{}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		return Value.NetSerialize(Ar, Map, bOutSuccess);
	}

	bool Identical(const FMeshAssetId* Other, uint32 PortFlags) const
	{
		return Other && Value.Identical(&Other->Value, PortFlags);
	}
};

template<>
struct TStructOpsTypeTraits<FMeshAssetId> : public TStructOpsTypeTraitsBase2<FMeshAssetId>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithZeroConstructor = true,
	};
};

USTRUCT(BlueprintType)
struct VWSHARED_API FMaterialId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 Value = 0;

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;
		Ar.SerializeIntPacked(Value);
		return true;
	}

	bool Identical(const FMaterialId* Other, uint32 /*PortFlags*/) const
	{
		return Other && Value == Other->Value;
	}
};

template<>
struct TStructOpsTypeTraits<FMaterialId> : public TStructOpsTypeTraitsBase2<FMaterialId>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithZeroConstructor = true,
	};
};

USTRUCT(BlueprintType)
struct VWSHARED_API FVMRepMeshSpatialItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpatialItemId Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMTransformNet Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMeshAssetId MeshAssetId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialId> MaterialIdsBySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasFlags = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Flags = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasParent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpatialItemId ParentId;

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		{
			bool bOk = true;
			Id.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		{
			bool bOk = true;
			Transform.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		{
			bool bOk = true;
			MeshAssetId.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		if (!bOutSuccess)
		{
			return false;
		}

		uint8 OptionalBits = 0;
		if (Ar.IsSaving())
		{
			OptionalBits |= (bHasFlags ? 1 << 0 : 0);
			OptionalBits |= (bHasParent ? 1 << 1 : 0);
		}
		Ar.SerializeBits(&OptionalBits, 2);

		if (Ar.IsLoading())
		{
			bHasFlags = (OptionalBits & (1 << 0)) != 0;
			bHasParent = (OptionalBits & (1 << 1)) != 0;
		}

		if (bHasFlags)
		{
			Ar.SerializeBits(&Flags, 8);
		}
		else if (Ar.IsLoading())
		{
			Flags = 0;
		}

		if (bHasParent)
		{
			bool bOk = true;
			ParentId.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		uint32 Count = static_cast<uint32>(MaterialIdsBySlot.Num());
		Ar.SerializeIntPacked(Count);

		if (Ar.IsLoading())
		{
			constexpr uint32 MaxMaterials = 64;
			Count = FMath::Min(Count, MaxMaterials);
			MaterialIdsBySlot.SetNum(static_cast<int32>(Count), EAllowShrinking::No);
		}

		for (uint32 Index = 0; Index < Count; ++Index)
		{
			bool bOk = true;
			MaterialIdsBySlot[static_cast<int32>(Index)].NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		return bOutSuccess;
	}

	bool Identical(const FVMRepMeshSpatialItem* Other, uint32 PortFlags) const
	{
		if (!Other)
		{
			return false;
		}

		if (!Id.Identical(&Other->Id, PortFlags))
		{
			return false;
		}

		if (Transform.Location != Other->Transform.Location)
		{
			return false;
		}

		if (!Transform.Rotation.Identical(&Other->Transform.Rotation, PortFlags))
		{
			return false;
		}

		if (Transform.bHasScale != Other->Transform.bHasScale)
		{
			return false;
		}

		if (Transform.bHasScale && Transform.Scale != Other->Transform.Scale)
		{
			return false;
		}

		if (!MeshAssetId.Identical(&Other->MeshAssetId, PortFlags))
		{
			return false;
		}

		if (bHasFlags != Other->bHasFlags || bHasParent != Other->bHasParent)
		{
			return false;
		}

		if (bHasFlags && Flags != Other->Flags)
		{
			return false;
		}

		if (bHasParent && !ParentId.Identical(&Other->ParentId, PortFlags))
		{
			return false;
		}

		const int32 Num = MaterialIdsBySlot.Num();
		if (Num != Other->MaterialIdsBySlot.Num())
		{
			return false;
		}

		for (int32 Index = 0; Index < Num; ++Index)
		{
			if (!MaterialIdsBySlot[Index].Identical(&Other->MaterialIdsBySlot[Index], PortFlags))
			{
				return false;
			}
		}

		return true;
	}
};

template<>
struct TStructOpsTypeTraits<FVMRepMeshSpatialItem> : public TStructOpsTypeTraitsBase2<FVMRepMeshSpatialItem>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};

UENUM(BlueprintType)
enum class EVMRepLightType : uint8
{
	Point UMETA(DisplayName="Point"),
	Spot UMETA(DisplayName="Spot"),
	Directional UMETA(DisplayName="Directional")
};

USTRUCT(BlueprintType)
struct VWSHARED_API FVMRepLightSpatialItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpatialItemId Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMTransformNet Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVMRepLightType Type = EVMRepLightType::Point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature = 6500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttenuationRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InnerConeAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OuterConeAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCastShadows = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCastVolumetricShadow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 LightingChannels = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVolumetric = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasIesProfileId = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMeshAssetId IesProfileId;

private:
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
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		{
			bool bOk = true;
			Id.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		{
			bool bOk = true;
			Transform.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		uint8 TypeBits = 0;
		if (Ar.IsSaving())
		{
			TypeBits = static_cast<uint8>(Type) & 0x3;
		}
		Ar.SerializeBits(&TypeBits, 2);
		if (Ar.IsLoading())
		{
			if (TypeBits > static_cast<uint8>(EVMRepLightType::Directional))
			{
				bOutSuccess = false;
				return false;
			}
			Type = static_cast<EVMRepLightType>(TypeBits);
		}

		Ar << Color;

		uint16 QIntensity = 0;
		if (Ar.IsSaving()) QIntensity = Quantize16(Intensity, 0.f, 200000.f);
		Ar << QIntensity;
		if (Ar.IsLoading()) Intensity = Dequantize16(QIntensity, 0.f, 200000.f);

		uint16 QTemp = 0;
		if (Ar.IsSaving()) QTemp = Quantize16(Temperature, 1000.f, 20000.f);
		Ar << QTemp;
		if (Ar.IsLoading()) Temperature = Dequantize16(QTemp, 1000.f, 20000.f);

		uint16 QAtten = 0;
		if (Ar.IsSaving()) QAtten = Quantize16(AttenuationRadius, 0.f, 50000.f);
		Ar << QAtten;
		if (Ar.IsLoading()) AttenuationRadius = Dequantize16(QAtten, 0.f, 50000.f);

		uint16 QInner = 0;
		uint16 QOuter = 0;
		if (Ar.IsSaving())
		{
			QInner = Quantize16(InnerConeAngle, 0.f, 90.f);
			QOuter = Quantize16(OuterConeAngle, 0.f, 90.f);
		}
		Ar << QInner;
		Ar << QOuter;
		if (Ar.IsLoading())
		{
			InnerConeAngle = Dequantize16(QInner, 0.f, 90.f);
			OuterConeAngle = Dequantize16(QOuter, 0.f, 90.f);
		}

		uint8 Flags = 0;
		if (Ar.IsSaving())
		{
			Flags |= (bCastShadows ? 1 << 0 : 0);
			Flags |= (bCastVolumetricShadow ? 1 << 1 : 0);
			Flags |= (bVolumetric ? 1 << 2 : 0);
			Flags |= (bHasIesProfileId ? 1 << 3 : 0);
			Flags |= static_cast<uint8>((LightingChannels & 0x7) << 4);
		}
		Ar.SerializeBits(&Flags, 7);
		if (Ar.IsLoading())
		{
			bCastShadows = (Flags & (1 << 0)) != 0;
			bCastVolumetricShadow = (Flags & (1 << 1)) != 0;
			bVolumetric = (Flags & (1 << 2)) != 0;
			bHasIesProfileId = (Flags & (1 << 3)) != 0;
			LightingChannels = static_cast<uint8>((Flags >> 4) & 0x7);
		}

		if (bHasIesProfileId)
		{
			bool bOk = true;
			IesProfileId.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		return bOutSuccess;
	}

	bool Identical(const FVMRepLightSpatialItem* Other, uint32 PortFlags) const
	{
		if (!Other)
		{
			return false;
		}

		if (!Id.Identical(&Other->Id, PortFlags))
		{
			return false;
		}

		if (Transform.Location != Other->Transform.Location)
		{
			return false;
		}

		if (!Transform.Rotation.Identical(&Other->Transform.Rotation, PortFlags))
		{
			return false;
		}

		if (Transform.bHasScale != Other->Transform.bHasScale)
		{
			return false;
		}

		if (Transform.bHasScale && Transform.Scale != Other->Transform.Scale)
		{
			return false;
		}

		if (Type != Other->Type
			|| Color != Other->Color
			|| bCastShadows != Other->bCastShadows
			|| bCastVolumetricShadow != Other->bCastVolumetricShadow
			|| bVolumetric != Other->bVolumetric
			|| LightingChannels != Other->LightingChannels
			|| bHasIesProfileId != Other->bHasIesProfileId)
		{
			return false;
		}

		auto Tol16 = [](float Range) { return (Range / 65535.f) * 2.f; };

		if (FMath::Abs(Intensity - Other->Intensity) > Tol16(200000.f))
		{
			return false;
		}

		if (FMath::Abs(Temperature - Other->Temperature) > Tol16(19000.f))
		{
			return false;
		}

		if (FMath::Abs(AttenuationRadius - Other->AttenuationRadius) > Tol16(50000.f))
		{
			return false;
		}

		if (FMath::Abs(InnerConeAngle - Other->InnerConeAngle) > Tol16(90.f))
		{
			return false;
		}

		if (FMath::Abs(OuterConeAngle - Other->OuterConeAngle) > Tol16(90.f))
		{
			return false;
		}

		if (bHasIesProfileId && !IesProfileId.Identical(&Other->IesProfileId, PortFlags))
		{
			return false;
		}

		return true;
	}
};

template<>
struct TStructOpsTypeTraits<FVMRepLightSpatialItem> : public TStructOpsTypeTraitsBase2<FVMRepLightSpatialItem>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};
