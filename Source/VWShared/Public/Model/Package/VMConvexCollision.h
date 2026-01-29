#pragma once
#include "CoreMinimal.h"
#include "VMConvexCollision.generated.h"

USTRUCT()
struct FVMConvexCollision
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector> vertices;
	
	FVMConvexCollision(): vertices({}) {}
	FVMConvexCollision(const TArray<FVector>& InVertices): 
		vertices(InVertices) {}
};