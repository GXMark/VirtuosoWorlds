#pragma once
#include "CoreMinimal.h"
#include "MPConvexCollision.generated.h"

USTRUCT()
struct FMPConvexCollision
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector> Vertices;
	
	FMPConvexCollision(): Vertices({}) {}
	FMPConvexCollision(const TArray<FVector>& InVertices): 
		Vertices(InVertices) {}
};