#pragma once
#include "CoreMinimal.h"
#include "VMSphereCollision.generated.h"

USTRUCT()
struct FVMSphereCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FVector center = FVector::ZeroVector;

	UPROPERTY()
	float radius = 0.f;
	
	FVMSphereCollision() {}
	FVMSphereCollision(const FVector& InCenter, float InRadius):
		center(InCenter)
		, radius(InRadius) {}
};
