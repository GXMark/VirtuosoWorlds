#pragma once

#include "CoreMinimal.h"
#include "VMBounds.generated.h"

USTRUCT()
struct VWSHARED_API FVMBounds
{
	GENERATED_BODY()

	UPROPERTY()
	FVector3f min;

	UPROPERTY()
	FVector3f max;

	FVMBounds(): min(), max()
	{
	}

	FVMBounds(const FVector3f& Position)
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