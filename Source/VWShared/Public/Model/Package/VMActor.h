#pragma once

#include "CoreMinimal.h"
#include "VMComponent.h"
#include "VMTransform.h"
#include "VMActor.generated.h"

USTRUCT()
struct VWSHARED_API FVMActor
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
	FVMTransform transform;
	
	UPROPERTY()
	FVMComponent component;

	FVMActor()	{}
	
	FVMActor(const FGuid& InID, const FGuid& InPID, const FName& InName, const FName& InType, const FVMTransform& InTransform, const FVMComponent& InComponent):
		id(InID)
		, pid(InPID)
		, name(InName)
		, type(InType)
		, transform(InTransform)
		, component(InComponent) 
	{}
};

