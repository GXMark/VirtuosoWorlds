#pragma once

#include "CoreMinimal.h"
#include "VMAssetRef.h"
#include "VMLinearColor.h"
#include "VMDecalComponent.generated.h"

USTRUCT()
struct VWSHARED_API FVMDecalComponent
{
	GENERATED_BODY()

	UPROPERTY()
	FVMAssetRef material_ref;

	UPROPERTY()
	FVector3d size;

	UPROPERTY()
	FVMLinearColor color;

	UPROPERTY()
	float fade_screen_size;

	UPROPERTY()
	float fade_in_start_delay;

	UPROPERTY()
	float fade_in_duration;

	UPROPERTY()
	float fade_out_start_delay;

	UPROPERTY()
	float fade_out_duration;

	UPROPERTY()
	bool destroy_after_fade_out;

	FVMDecalComponent(): 
		material_ref({}), 
		size(0,0,0), 
		color({}), 
		fade_screen_size(0), 
		fade_in_start_delay(0), 
		fade_in_duration(0), 
		fade_out_start_delay(0), 
		fade_out_duration(0), 
		destroy_after_fade_out(false)
	{}
	
	FVMDecalComponent(const FVMAssetRef& InMaterialRef, const FVector3d& InSize, const FVMLinearColor& InColor, const float InFadeScreenSize, const float InFadeInStartDelay, const float InFadeInDuration, const float InFadeOutStartDelay, const float InFadeOutDuration, const bool InDestroyAfterFadeOut):
		material_ref(InMaterialRef),
		size(InSize),
		color(InColor),
		fade_screen_size(InFadeScreenSize),
		fade_in_start_delay(InFadeInStartDelay),
		fade_in_duration(InFadeInDuration),
		fade_out_start_delay(InFadeOutStartDelay),
		fade_out_duration(InFadeOutDuration),
		destroy_after_fade_out(InDestroyAfterFadeOut)
	{}
};