#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Model/Network/VMMaterialNet.h"
#include "VReplicatedMaterialFastArray.generated.h"

class AVMaterialChunk;

USTRUCT()
struct VWSHARED_API FVReplicatedMaterialItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FVMMaterialNet Material;

	// We want the wrapper to serialize exactly like FVMMaterialNet
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		return Material.NetSerialize(Ar, Map, bOutSuccess);
	}

	// FastArray callbacks (defined in VMaterialChunk.cpp to avoid circular includes)
	void PreReplicatedRemove(const struct FVReplicatedMaterialItemArray& InArray);
	void PostReplicatedAdd(const struct FVReplicatedMaterialItemArray& InArray);
	void PostReplicatedChange(const struct FVReplicatedMaterialItemArray& InArray);
};

template<>
struct TStructOpsTypeTraits<FVReplicatedMaterialItem> : public TStructOpsTypeTraitsBase2<FVReplicatedMaterialItem>
{
	enum { WithNetSerializer = true, WithCopy = true };
};

USTRUCT()
struct VWSHARED_API FVReplicatedMaterialItemArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVReplicatedMaterialItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<AVMaterialChunk> Owner = nullptr;

	void SetOwner(AVMaterialChunk* InOwner) { Owner = InOwner; }

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FVReplicatedMaterialItem, FVReplicatedMaterialItemArray>(
			Items, DeltaParms, *this
		);
	}
};

template<>
struct TStructOpsTypeTraits<FVReplicatedMaterialItemArray> : public TStructOpsTypeTraitsBase2<FVReplicatedMaterialItemArray>
{
	enum { WithNetDeltaSerializer = true };
};
