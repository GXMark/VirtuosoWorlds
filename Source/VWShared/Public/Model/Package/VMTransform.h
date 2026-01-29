#pragma once
#include "CoreMinimal.h"
#include "VMTransform.generated.h"

USTRUCT()
struct FVMTransform
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Position = FVector::ZeroVector;

	UPROPERTY()
	FVector Rotation = FVector::ZeroVector;

	UPROPERTY()
	FVector Scale = FVector::OneVector;
	
	FVMTransform() {}
	
	FVMTransform(const FVector& InPosition, const FVector& InRotation, const FVector& InScale):
		Position(InPosition)
		, Rotation(InRotation)
		, Scale(InScale) {}
	
	FVMTransform(const FTransform& InTransform):
		Position(InTransform.GetLocation())
		, Rotation(InTransform.GetRotation().Euler())
		, Scale(InTransform.GetScale3D())
	{}
	
	FVMTransform Quantize() const
	{
		return FVMTransform(Position,Rotation,Scale);
	}
};


