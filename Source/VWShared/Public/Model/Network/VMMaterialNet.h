#pragma once

#include "CoreMinimal.h"
#include "VMGuidNet.h"
#include "VMLinearColorNet.h"
#include "VMScalarNet.h"
#include "VMTextureNet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "VMMaterialNet.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FVMMaterialNet : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FVMGuidNet id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName parent;

	UPROPERTY(EditAnywhere, Category="Materials", meta=(ShowOnlyInnerProperties))
	TArray<FVMScalarNet> scalars;

	UPROPERTY(EditAnywhere, Category="Materials", meta=(ShowOnlyInnerProperties))
	TArray<FVMLinearColorNet> vectors;

	UPROPERTY(EditAnywhere, Category="Materials", meta=(ShowOnlyInnerProperties))
	TArray<FVMTextureNet> textures;

	FVMMaterialNet()
		: id(FGuid(0, 0, 0, 0))
		, name(NAME_None)
		, parent(NAME_None)
	{}

	FVMMaterialNet(const FGuid& ID, const FString& Name, const FString& Parent,
		const TArray<FVMScalarNet>& Scalars,
		const TArray<FVMLinearColorNet>& Vectors,
		const TArray<FVMTextureNet>& Textures)
		: id(ID)
		, name(*Name)
		, parent(*Parent)
		, scalars(Scalars)
		, vectors(Vectors)
		, textures(Textures)
	{}

	// ---- NetSerialize ----
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true;

		// id (your optimized struct)
		{
			bool bOk = true;
			id.NetSerialize(Ar, Map, bOk);
			bOutSuccess &= bOk;
		}

		// Names (FName is already net-optimized/deduped per connection)
		Ar << name;
		Ar << parent;

		// --- scalars ---
		{
			uint32 Count = scalars.Num();
			Ar.SerializeIntPacked(Count);

			if (Ar.IsLoading())
			{
				scalars.SetNum(static_cast<int32>(Count), EAllowShrinking::No);
			}

			for (uint32 i = 0; i < Count; ++i)
			{
				bool bOk = true;
				scalars[static_cast<int32>(i)].NetSerialize(Ar, Map, bOk);
				bOutSuccess &= bOk;
			}
		}

		// --- vectors ---
		{
			uint32 Count = vectors.Num();
			Ar.SerializeIntPacked(Count);

			if (Ar.IsLoading())
			{
				vectors.SetNum(static_cast<int32>(Count), EAllowShrinking::No);
			}

			for (uint32 i = 0; i < Count; ++i)
			{
				bool bOk = true;
				vectors[static_cast<int32>(i)].NetSerialize(Ar, Map, bOk);
				bOutSuccess &= bOk;
			}
		}

		// --- textures ---
		{
			uint32 Count = textures.Num();
			Ar.SerializeIntPacked(Count);

			if (Ar.IsLoading())
			{
				textures.SetNum(static_cast<int32>(Count), EAllowShrinking::No);
			}

			for (uint32 i = 0; i < Count; ++i)
			{
				bool bOk = true;
				textures[static_cast<int32>(i)].NetSerialize(Ar, Map, bOk);
				bOutSuccess &= bOk;
			}
		}

		return bOutSuccess;
	}
};

// ---- TypeTraits ----
template<>
struct TStructOpsTypeTraits<FVMMaterialNet> : public TStructOpsTypeTraitsBase2<FVMMaterialNet>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
		WithNetSharedSerialization = true
	};
};
