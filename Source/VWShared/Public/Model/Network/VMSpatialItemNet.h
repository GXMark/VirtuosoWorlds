#pragma once
#include "CoreMinimal.h"
#include "Model/Network/VMMeshComponentNet.h"
#include "Model/Network/VMPointLightComponentNet.h"
#include "Model/Network/VMSpotLightComponentNet.h"
#include "Model/Network/VMTransformNet.h"
#include "VMSpatialItemNet.generated.h"

UENUM(BlueprintType)
enum class ESpatialItemType : uint8
{
	Mesh       UMETA(DisplayName="Mesh"),
	PointLight UMETA(DisplayName="PointLight"),
	SpotLight  UMETA(DisplayName="SpotLight"),
};

USTRUCT(BlueprintType)
struct VWSHARED_API FVMSpatialItemNet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMGuidNet ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMGuidNet ParentID;

	// Shared collision definition id (applies regardless of payload type)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVMGuidNet CollisionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpatialItemType PayloadType = ESpatialItemType::Mesh;

	UPROPERTY()
	FVMMeshComponentNet MeshPayload;

	UPROPERTY()
	FVMPointLightComponentNet PointLightPayload;

	UPROPERTY()
	FVMSpotLightComponentNet SpotLightPayload;

	UPROPERTY()
	FVMTransformNet Transform;

	FVMSpatialItemNet()
		: ItemID(FGuid(0,0,0,0)) // IMPORTANT: don't generate random IDs on clients
		, CollisionID(FGuid(0,0,0,0))
	{}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// --- Identity & hierarchy ---
		{
			bool bOk = true; ItemID.NetSerialize(Ar, Map, bOk);   bOutSuccess &= bOk;
		}
		{
			bool bOk = true; ParentID.NetSerialize(Ar, Map, bOk); bOutSuccess &= bOk;
		}
		{
			bool bOk = true; CollisionID.NetSerialize(Ar, Map, bOk); bOutSuccess &= bOk;
		}
		if (!bOutSuccess) return false;

		// --- Payload type (2 bits supports up to 4 types) ---
		uint8 PayloadTypeBits = 0;
		if (Ar.IsSaving())
		{
			PayloadTypeBits = static_cast<uint8>(PayloadType) & 0x3;
		}
		Ar.SerializeBits(&PayloadTypeBits, 2);

		if (Ar.IsLoading())
		{
			// Defensive: map unknown values to Mesh (or fail)
			if (PayloadTypeBits > static_cast<uint8>(ESpatialItemType::SpotLight))
			{
				bOutSuccess = false;
				return false;
			}
			PayloadType = static_cast<ESpatialItemType>(PayloadTypeBits);
		}

		// --- Transform ---
		{
			bool bOk = true;
			Transform.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}
		if (!bOutSuccess) return false;

		// --- Payload (only one is serialized) ---
		switch (PayloadType)
		{
		case ESpatialItemType::Mesh:
		{
			bool bOk = true;
			MeshPayload.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
			break;
		}
		case ESpatialItemType::PointLight:
		{
			bool bOk = true;
			PointLightPayload.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
			break;
		}
		case ESpatialItemType::SpotLight:
		{
			bool bOk = true;
			SpotLightPayload.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
			break;
		}
		default:
			bOutSuccess = false;
			return false;
		}

		return bOutSuccess;
	}
};

template<>
struct TStructOpsTypeTraits<FVMSpatialItemNet> : public TStructOpsTypeTraitsBase2<FVMSpatialItemNet>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
