#pragma once
#include "CoreMinimal.h"
#include "MPTransform.generated.h"

// Human readable authoring transform (not network efficient)
USTRUCT()
struct FMPTransform
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Position = FVector::ZeroVector;

	UPROPERTY()
	FVector Rotation = FVector::ZeroVector;

	UPROPERTY()
	FVector Scale = FVector::OneVector;
	
	FMPTransform() {}
	
	FMPTransform(const FVector& InPosition, const FVector& InRotation, const FVector& InScale):
		Position(InPosition)
		, Rotation(InRotation)
		, Scale(InScale) {}
	
	FMPTransform(const FTransform& InTransform):
		Position(InTransform.GetLocation())
		, Rotation(InTransform.GetRotation().Euler())
		, Scale(InTransform.GetScale3D())
	{}
};

