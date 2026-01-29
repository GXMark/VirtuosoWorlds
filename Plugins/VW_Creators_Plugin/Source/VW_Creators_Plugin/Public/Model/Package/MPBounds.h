#pragma once

#include "CoreMinimal.h"
#include "MPBounds.generated.h"

USTRUCT()
struct FMPBounds
{
	GENERATED_BODY()

	UPROPERTY()
	FVector3f min;

	UPROPERTY()
	FVector3f max;

	FMPBounds(): min(), max()
	{
	}

	FMPBounds(const FVector& Position)
	{
		min.X = Position.X;
		min.Y = Position.Y;
		min.Z = Position.Z;
		max.X = Position.X;
		max.Y = Position.Y;
		max.Z = Position.Z;
	}

	FVector3f Center() const
	{
		return FVector3f((min.X + max.X) / 2,(min.Y + max.Y) / 2,(min.Z + max.Z) / 2);
	}
};
