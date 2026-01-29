#pragma once

#include "CoreMinimal.h"
#include "VMGuidNet.h"
#include "VMAssetRefNet.generated.h"

/*
 * FVMAssetRefNet
 * Canonical network representation of an authored asset reference.
 *
 * Notes:
 * - This is the transport form used by replicated payloads / RPCs.
 * - The authored/package form (FVMAssetRef) is used for data ingestion and tooling.
 * - This struct should remain lean; type-specific resolution flags should be modeled explicitly
 *   by the referencing payload (e.g., texture refs carry a dedicated 'linear' flag).
 */
USTRUCT()
struct VWSHARED_API FVMAssetRefNet
{
	GENERATED_BODY()

	UPROPERTY()
	FVMGuidNet id;

	UPROPERTY()
	FName name;

	UPROPERTY()
	FName type;

	// Authoritative editor asset reference from FVMAssetRef.path (may be NAME_None in packaged builds)
	UPROPERTY()
	FName path;

	FVMAssetRefNet()
		: id({})
		, name(NAME_None)
		, type(NAME_None)
		, path(NAME_None)
	{}

	FVMAssetRefNet(const FGuid& InId, const FName& InName, const FName& InType, const FName& InPath)
		: id(InId)
		, name(InName)
		, type(InType)
		, path(InPath)
	{}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// Id
		{
			bool bOk = true;
			id.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// Names
		Ar << name;
		Ar << type;
		Ar << path;

		return true;
	}

	// ---- Identical ----
	bool Identical(const FVMAssetRefNet* Other, uint32 /*PortFlags*/) const
	{
		return Other
			&& id.Identical(&Other->id, 0)
			&& name == Other->name
			&& type == Other->type
			&& path == Other->path
			;
	}
};

// Enable NetSerializer for FVMAssetRefNet
template<>
struct TStructOpsTypeTraits<FVMAssetRefNet> : public TStructOpsTypeTraitsBase2<FVMAssetRefNet>
{
	enum
	{
		WithNetSerializer = true,
		WithIdentical = true,
		WithCopy = true
	};
};
