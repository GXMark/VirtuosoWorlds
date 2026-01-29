#pragma once

#include "CoreMinimal.h"
#include "VMScalar.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FVMScalar
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	float value;

	FVMScalar():
		name(NAME_None)
		, value(0) 
	{}
	
	FVMScalar(const FName& Name, const float Value) : 
		name(Name)
		, value(Value)
	{}
};