#pragma once
#include "CoreMinimal.h"
#include "VMCapsuleCollision.generated.h"

USTRUCT()
struct FVMCapsuleCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FVector center = FVector::ZeroVector;

	UPROPERTY()
	FRotator rotation = FRotator::ZeroRotator;

	UPROPERTY()
	float radius = 0.f;

	UPROPERTY()
	float length = 0.f;
	
	FVMCapsuleCollision() {}
	FVMCapsuleCollision(const FVector& InCenter, const FRotator& InRotation, float InRadius, float InLength):
		center(InCenter)
		, rotation(InRotation)
		, radius(InRadius)
		, length(InLength) {}
};