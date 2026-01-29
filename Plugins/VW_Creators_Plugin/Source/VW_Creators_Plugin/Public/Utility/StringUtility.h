#pragma once
#include "CoreMinimal.h"

class FCPStringUtility
{
public:
	static FVector3d RemoveNegativeZero(const FVector3d& InValue);
	static FString ConvertUInt32ArrayToString(const TArray<uint32>& InArray);
	static TArray<uint32> ConvertStringToUInt32Array(const FString& InValueStr);
};
