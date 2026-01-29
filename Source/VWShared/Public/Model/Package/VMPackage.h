#pragma once

#include "CoreMinimal.h"
#include "VMActor.h"
#include "VMMaterial.h"
#include "VMPackage.generated.h"

USTRUCT()
struct VWSHARED_API FVMPackage
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
	TArray<FVMActor> actors; // root actors (static mesh, hism, decals, lights, etc)

	UPROPERTY()
	TArray<FVMMaterial> materials; // shared materials supporting root, hism and group actors

	FVMPackage() :
		id(FGuid(0, 0, 0, 0))
		, name(NAME_None)
		, description(NAME_None)
		, author(NAME_None)
		, engine(NAME_None)
		, created(NAME_None)
		, actors({})
		, materials({})
	{}

	FVMPackage(const FGuid& ID, const FName& Name, const FName& Description, const FName& Author, const FName& Version, const FName& Created, const TArray<FVMActor>& Actors, const TArray<FVMMaterial>& Materials) :
		id(ID)
		, name(Name)
		, description(Description)
		, author(Author)
		, engine(Version)
		, created(Created)
		, actors(Actors)
		, materials(Materials)
	{}
};
