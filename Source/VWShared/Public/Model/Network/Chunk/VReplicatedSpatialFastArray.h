#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "VReplicatedSpatialFastArray.generated.h"

class AVReplicatedChunk;

/**
 * Wrapper item so FVSpatialItem can participate in a FastArray.
 * (FVSpatialItem already NetSerializes; we wrap it with FastArraySerializerItem bookkeeping.)
 */
USTRUCT()
struct VWSHARED_API FVReplicatedSpatialItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FVMSpatialItemNet Item;

	// FastArray callbacks (defined in VReplicatedChunk.cpp to avoid circular includes)
	void PreReplicatedRemove(const struct FVReplicatedSpatialItemArray& InArray) const;
	void PostReplicatedAdd(const struct FVReplicatedSpatialItemArray& InArray) const;
	void PostReplicatedChange(const struct FVReplicatedSpatialItemArray& InArray) const;
};

USTRUCT()
struct VWSHARED_API FVReplicatedSpatialItemArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVReplicatedSpatialItem> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<AVReplicatedChunk> Owner = nullptr;

	void SetOwner(AVReplicatedChunk* InOwner) { Owner = InOwner; }

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FVReplicatedSpatialItem, FVReplicatedSpatialItemArray>(
			Items, DeltaParms, *this
		);
	}
};

template<>
struct TStructOpsTypeTraits<FVReplicatedSpatialItemArray> : public TStructOpsTypeTraitsBase2<FVReplicatedSpatialItemArray>
{
	enum { WithNetDeltaSerializer = true };
};
