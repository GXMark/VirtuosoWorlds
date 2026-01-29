#pragma once
#include "CoreMinimal.h"

class VW_CREATORS_PLUGIN_API FCPPAKUtility
{
public:
	static void CookCurrentLevel();
	static void CreatePAK(const FGuid& InAssetID, const FName& InAssetType, const FName& InAssetName, const FName& InAssetPath);
	static bool CreatePAKAssetLoadPath(const FString& InMountPoint, const FString& InAssetSourcePath, FString& OutPAKLoadPath);
private:
	static FString PrepareAssetPath(const FName& InAssetPath);
};
