#pragma once
#include "CoreMinimal.h"
#include "MPCapsuleCollision.generated.h"

USTRUCT()
struct FMPCapsuleCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Center = FVector::ZeroVector;

	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY()
	float Radius = 0.f;

	UPROPERTY()
	float Length = 0.f;
	
	FMPCapsuleCollision() {}
	FMPCapsuleCollision(const FVector& InCenter, const FRotator& InRotation, float InRadius, float InLength):
		Center(InCenter)
		, Rotation(InRotation)
		, Radius(InRadius)
		, Length(InLength) {}
};