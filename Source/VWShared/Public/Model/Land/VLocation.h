#pragma once

#include "CoreMinimal.h"
#include "VLocation.generated.h"

USTRUCT()
struct VWSHARED_API FVLocation
{
	GENERATED_BODY()

	UPROPERTY()
	FString region_name;	// region name

	UPROPERTY()
	FGuid region_id;		// region guid
	
	UPROPERTY()
	FString parcel_name;	// parcel name
	
	UPROPERTY()
	FGuid parcel_id;		// parcel guid

	FVLocation() {}

	FVLocation(const FString& RegionName, const FGuid& RegionID, const FString& ParcelName, const FGuid& ParcelID):
		region_name(RegionName), region_id(RegionID), parcel_name(ParcelName), parcel_id(ParcelID)
	{
	}
};
