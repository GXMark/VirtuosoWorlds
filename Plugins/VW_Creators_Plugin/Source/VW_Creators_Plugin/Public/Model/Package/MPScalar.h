#pragma once

#include "CoreMinimal.h"
#include "MPScalar.generated.h"

USTRUCT()
struct FMPScalar
{
	GENERATED_BODY()

	UPROPERTY()
	FName name;

	UPROPERTY()
	float value;

	FMPScalar():
		name(NAME_None)
		, value(0) 
	{}
	
	FMPScalar(const FName& InName, const float InValue) : 
		name(InName)
		, value(InValue) 
	{}
};
