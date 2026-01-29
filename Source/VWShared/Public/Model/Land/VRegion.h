#pragma once

#include "CoreMinimal.h"
#include "VParcel.h"
#include "VRegion.generated.h"

USTRUCT()
struct VWSHARED_API FVRegion
{
	GENERATED_BODY()

	UPROPERTY()
	FString name; // region friendly name

	UPROPERTY()
	FGuid id; // region guid

	UPROPERTY()
	FString description; // region description

	UPROPERTY()
	FString owner_id; // region owner identifier

	UPROPERTY()
	FIntVector size; // region size x - width, y - depth, z - height

	UPROPERTY()
	TArray<FVParcel> parcels; // region parcels

	UPROPERTY()
	int32 land_impact; // region land impact [ parcel land impact must total this ]

	FVRegion(): size(), land_impact(0)
	{
	}

	FVRegion(const FString& Name, const FGuid& ID, const FString& Description, const FString& OwnerID, const FIntVector& Size, const TArray<FVParcel>& Parcels, const int32 LandImpact):
		name(Name), id(ID), description(Description), owner_id(OwnerID), size(Size), parcels(Parcels), land_impact(LandImpact)
	{
	}
};
