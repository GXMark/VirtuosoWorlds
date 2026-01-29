#pragma once
#include "CoreMinimal.h"
#include "VMBoxCollision.generated.h"

USTRUCT()
struct FVMBoxCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Center = FVector::ZeroVector;

	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY()
	FVector Extents = FVector::ZeroVector; // X,Y,Z
	
	FVMBoxCollision() {}
	FVMBoxCollision(const FVector& InCenter, const FRotator& InRotation, const FVector& InExtents):
		Center(InCenter)
		, Rotation(InRotation)
		, Extents(InExtents) {}
};
