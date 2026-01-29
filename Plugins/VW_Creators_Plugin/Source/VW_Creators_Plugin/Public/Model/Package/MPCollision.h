#pragma once
#include "CoreMinimal.h"
#include "MPBoxCollision.h"
#include "MPCapsuleCollision.h"
#include "MPConvexCollision.h"
#include "MPSphereCollision.h"
#include "MPCollision.generated.h"

USTRUCT()
struct FMPCollision
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGuid MeshID;

	UPROPERTY()
	TArray<FMPSphereCollision> Spheres;

	UPROPERTY()
	TArray<FMPBoxCollision> Boxes;

	UPROPERTY()
	TArray<FMPCapsuleCollision> Capsules;

	UPROPERTY()
	TArray<FMPConvexCollision> Convexes;
	
	FMPCollision(): MeshID(FGuid(0,0,0,0)) {}
	FMPCollision(const FGuid& InMeshID, const TArray<FMPSphereCollision>& InSpheres, const TArray<FMPBoxCollision>& InBoxes, const TArray<FMPCapsuleCollision>& InCapsules, const TArray<FMPConvexCollision>& InConvexes): 
		MeshID(InMeshID)
		, Spheres(InSpheres)
		, Boxes(InBoxes)
		, Capsules(InCapsules)
		, Convexes(InConvexes) {}
};


