#pragma once

#include "CoreMinimal.h"
#include "VParcel.generated.h"

USTRUCT()
struct VWSHARED_API FVParcel
{
	GENERATED_BODY()

	UPROPERTY()
	FString name; // parcel friendly name
	
	UPROPERTY()
	FGuid id;	// parcel guid

	UPROPERTY()
	FString description; // parcel description

	UPROPERTY()
	FString owner; // parcel owner
	
	UPROPERTY()
	FIntVector size;	// parcel size x - width, y - depth, z - height

	UPROPERTY()
	FIntVector position; // parcel offset x - width, y - depth, z - height

	UPROPERTY()
	FVector3f landing_position; // default landing position
	
	UPROPERTY()
	FVector3f landing_rotation; // default landing rotation

	UPROPERTY()
	int32 max_players;  // parcel maximum concurrent players
	
	UPROPERTY()
	int32 land_impact;  // parcel land impact

	FVParcel(): size(), position(), landing_position(), landing_rotation(), max_players(0), land_impact(0)
	{
	}
	
	FVParcel(const FString& Name, const FGuid& ID, const FString& Description, const FString& Owner, const FIntVector& Size, const FIntVector& Position, const FVector3f& LandingPosition, const FVector3f& LandingRotation, const int32 MaxPlayers, const int32 LandImpact):
		name(Name), id(ID), description(Description), owner(Owner), size(Size), position(Position), landing_position(LandingPosition), landing_rotation(LandingRotation), max_players(MaxPlayers), land_impact(LandImpact)
	{
	}
};
