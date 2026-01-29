#pragma once
#include "CoreMinimal.h"
#include "MPSphereCollision.generated.h"

USTRUCT()
struct FMPSphereCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Center = FVector::ZeroVector;

	UPROPERTY()
	float Radius = 0.f;
	
	FMPSphereCollision() {}
	FMPSphereCollision(const FVector& InCenter, float InRadius):
		Center(InCenter)
		, Radius(InRadius) {}
};
