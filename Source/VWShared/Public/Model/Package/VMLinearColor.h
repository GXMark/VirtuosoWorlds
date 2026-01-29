#pragma once

#include "CoreMinimal.h"
#include "VMLinearColor.generated.h"

USTRUCT(BlueprintType)
struct VWSHARED_API FVMLinearColor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FName name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Material")
	FLinearColor color;

	FVMLinearColor() : 
		name(NAME_None)
		,color(0, 0, 0, 0)
	{}
	
	FVMLinearColor(const FName& Name, const FLinearColor& Color) :
		name(Name)
		, color(Color)
	{}
	
	FVMLinearColor(const FLinearColor& Color) : 
		name(NAME_None)
		, color(Color)
	{}
};