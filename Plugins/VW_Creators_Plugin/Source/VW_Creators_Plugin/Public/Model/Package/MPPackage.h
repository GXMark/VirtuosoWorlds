#pragma once

#include "CoreMinimal.h"
#include "MPActor.h"
#include "MPCollision.h"
#include "MPMaterial.h"
#include "MPPackage.generated.h"

USTRUCT()
struct FMPPackage
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid id;
	
	UPROPERTY()
	FName name;

	UPROPERTY()
	FName description;
	
	UPROPERTY()
	FName author;
	
	UPROPERTY()
	FName engine;

	UPROPERTY()
	FName created;
	
	UPROPERTY()
	TArray<FMPActor> actors;			// root actors (static mesh, hism, decal, light, etc.)

	UPROPERTY()
	TArray<FMPMaterial> materials;		// shared materials supporting root, hism and group actors

	UPROPERTY()
	TArray<FMPCollision> collisions;	// shared mesh primitive collision data

	
	FMPPackage(): 
		id(FGuid(0, 0, 0, 0))
		, name(TEXT(""))
		, description(TEXT(""))
		, author(TEXT(""))
		, engine(TEXT(""))
		, created(TEXT(""))
		, actors({})
		, materials({})
		, collisions({})
	{}
	
	FMPPackage(const FGuid& InID, const FName& InName, const FName& InDescription, const FName& InAuthor, const FName& InVersion, const FName& InCreated, const TArray<FMPActor>& InActors, const TArray<FMPMaterial>& InMaterials, const TArray<FMPCollision>& InCollisions):
		id(InID)
		, name(InName)
		, description(InDescription)
		, author(InAuthor)
		, engine(InVersion)
		, created(InCreated)
		, actors(InActors)
		, materials(InMaterials)
		, collisions(InCollisions)
	{}
};
