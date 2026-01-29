#pragma once
#include "CoreMinimal.h"
#include "VMBoxCollision.h"
#include "VMCapsuleCollision.h"
#include "VMConvexCollision.h"
#include "VMSphereCollision.h"
#include "VMCollision.generated.h"

USTRUCT()
struct FVMCollision
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGuid id;

	UPROPERTY()
	TArray<FVMSphereCollision> spheres;

	UPROPERTY()
	TArray<FVMBoxCollision> boxes;

	UPROPERTY()
	TArray<FVMCapsuleCollision> capsules;

	UPROPERTY()
	TArray<FVMConvexCollision> convexes;
	
	FVMCollision(): id(FGuid(0,0,0,0)) {}
	FVMCollision(const FGuid& InID, const TArray<FVMSphereCollision>& InSpheres, const TArray<FVMBoxCollision>& InBoxes, const TArray<FVMCapsuleCollision>& InCapsules, const TArray<FVMConvexCollision>& InConvexes): 
		id(InID)
		, spheres(InSpheres)
		, boxes(InBoxes)
		, capsules(InCapsules)
		, convexes(InConvexes) {}
};


