#pragma once

#include "CoreMinimal.h"
#include "MPTexture.generated.h"

USTRUCT()
struct FMPTexture
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid id;
	
	UPROPERTY()
	FName name;
	
	UPROPERTY()
	FName property;

	UPROPERTY()
	int32 width;

	UPROPERTY()
	int32 height;

	UPROPERTY()
	bool linear;

	UPROPERTY()
	FName format;

	FMPTexture() : id(FGuid(0,0,0,0)), name(NAME_None), property(NAME_None), width(0), height(0), linear(false), format(NAME_None) {}

	FMPTexture(const FGuid& InID, const FName& InProperty, const FName& InName, const int32 InWidth, const int32 InHeight, const bool InLinear, const FName& InFormat):
		id(InID)
		, name(InName)
		, property(InProperty)
		, width(InWidth)
		, height(InHeight)
		, linear(InLinear)
		, format(InFormat)
	{}
};
