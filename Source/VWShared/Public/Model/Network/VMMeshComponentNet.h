#pragma once

#include "CoreMinimal.h"
#include "VMAssetRefNet.h"
#include "VMGuidNet.h"
#include "VMMeshComponentNet.generated.h"

USTRUCT()
struct VWSHARED_API FVMMeshComponentNet
{
	GENERATED_BODY()

	// Canonical mesh asset reference (no fragmented fields)
	UPROPERTY()
	FVMAssetRefNet mesh_ref;

	UPROPERTY()
	TArray<FVMGuidNet> material_ids;

	FVMMeshComponentNet()
		: mesh_ref()
		  , material_ids({})
	{
	}

	FVMMeshComponentNet(const FVMAssetRefNet& InMeshRef, const TArray<FVMGuidNet>& InMaterialIds)
		: mesh_ref(InMeshRef)
		  , material_ids(InMaterialIds)
	{
	}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// Mesh reference
		{
			bool bOk = true;
			mesh_ref.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// Material references array
		uint32 Count = static_cast<uint32>(material_ids.Num());
		Ar.SerializeIntPacked(Count);

		if (Ar.IsLoading())
		{
			// Optional safety cap
			constexpr uint32 MaxMaterials = 64;
			Count = FMath::Min(Count, MaxMaterials);

			material_ids.SetNum(static_cast<int32>(Count), EAllowShrinking::No);
		}

		for (uint32 i = 0; i < Count; ++i)
		{
			bool bOk = true;
			material_ids[static_cast<int32>(i)].NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		return bOutSuccess;
	}

	// ---- Identical ----
	bool Identical(const FVMMeshComponentNet* Other, uint32 PortFlags) const
	{
		if (!Other) return false;

		if (!mesh_ref.Identical(&Other->mesh_ref, PortFlags))
			return false;

		const int32 Num = material_ids.Num();
		if (Num != Other->material_ids.Num())
			return false;

		for (int32 i = 0; i < Num; ++i)
		{
			if (!material_ids[i].Identical(&Other->material_ids[i], PortFlags))
				return false;
		}

		return true;
	}
};

// Enable NetSerializer for FVMMeshComponentNet
template <>
struct TStructOpsTypeTraits<FVMMeshComponentNet> : public TStructOpsTypeTraitsBase2<FVMMeshComponentNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};
