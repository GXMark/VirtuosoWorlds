#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VCommonUtilsBlueprintLibrary.generated.h"

UCLASS()
class UVCommonUtilsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//! Allow the blueprint to determine whether we are running with the editor or not
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "CommonUtilsFunctionLibrary")
	static bool RunningInPIE()
	{
#if WITH_EDITOR
		return true;
#else
		return false;
#endif
	}
};
