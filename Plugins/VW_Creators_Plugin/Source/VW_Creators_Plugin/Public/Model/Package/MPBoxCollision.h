#pragma once
#include "CoreMinimal.h"
#include "MPBoxCollision.generated.h"

USTRUCT()
struct FMPBoxCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Center = FVector::ZeroVector;

	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY()
	FVector Extents = FVector::ZeroVector; // X,Y,Z
	
	FMPBoxCollision() {}
	FMPBoxCollision(const FVector& InCenter, const FRotator& InRotation, const FVector& InExtents):
		Center(InCenter)
		, Rotation(InRotation)
		, Extents(InExtents) {}
};
