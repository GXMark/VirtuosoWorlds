#pragma once
#include "CoreMinimal.h"

class VWSHARED_API FVFileUtility
{
public:
	static bool EnsureEmptyDirectory(const FString& AbsolutePath);
	static bool LoadByteArrayFromFile(const FString& InAbsolutePath, const FGuid& InID, const FName& InName, TArray<uint8>& OutData);
	static bool SaveAssetAsByteArrayToFile(const FString& InAbsolutePath, const FName& InAssetType, const FGuid& InAssetID, const TArray<uint8>& InData);
};
