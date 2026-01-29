#pragma once

#include "CoreMinimal.h"
#include "MPComponent.h"
#include "MPTransform.h"
#include "MPActor.generated.h"

USTRUCT()
struct FMPActor
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid id;

	UPROPERTY()
	FGuid pid;

	UPROPERTY()
	FName name;

	UPROPERTY()
	FName type;
    
	UPROPERTY()
	FMPTransform transform;
	
	UPROPERTY()
	FMPComponent component;	// Actor component type

	FMPActor()	{}
	
	FMPActor(const FGuid& InID, const FGuid& InPID, const FName& InName, const FName& InType, const FTransform& InTransform, const FMPComponent& InComponent):
		id(InID)
		, pid(InPID)
		, name(InName)
		, type(InType)
		, transform(InTransform)
		, component(InComponent) 
	{}
};
