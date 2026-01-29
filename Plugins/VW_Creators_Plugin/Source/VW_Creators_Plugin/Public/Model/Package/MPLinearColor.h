#pragma once

#include "CoreMinimal.h"
#include "MPLinearColor.generated.h"

USTRUCT()
struct FMPLinearColor
{
	GENERATED_BODY()

	UPROPERTY()
	FName name;
	
	UPROPERTY()
	FLinearColor color;

	FMPLinearColor() : 
		name(NAME_None)
		, color(0, 0, 0, 0) 
	{}
	
	FMPLinearColor(const FName& Name, const FLinearColor& Color) : 
		name(Name)
		, color(Color) 
	{}
	
	FMPLinearColor(const FLinearColor& Color) : 
		name(NAME_None)
		, color(Color) 
	{}
};
