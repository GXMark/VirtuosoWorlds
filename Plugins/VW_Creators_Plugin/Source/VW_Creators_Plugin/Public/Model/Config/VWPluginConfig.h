#pragma once

#include "CoreMinimal.h"
#include "VWPluginConfig.generated.h"

USTRUCT(BlueprintType)
struct FVWPluginConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool ClearLogs;
	UPROPERTY(BlueprintReadWrite)
	bool ShowLogs;
	UPROPERTY(BlueprintReadWrite)
	bool ShowJson;
	UPROPERTY(BlueprintReadWrite)
	bool ClearAllAssets;
	UPROPERTY(BlueprintReadWrite)
	bool DontPackageAssets;
	UPROPERTY(BlueprintReadWrite)
	FString PackageName;
	UPROPERTY(BlueprintReadWrite)
	FString PackageDescription;
	UPROPERTY(BlueprintReadWrite)
	FString PackageAuthor;
	UPROPERTY(BlueprintReadWrite)
	FString PackagePath;
	
	FVWPluginConfig(): ClearLogs(false), ShowLogs(false), ShowJson(false), ClearAllAssets(false), DontPackageAssets(false)
	{
	}

	FVWPluginConfig(
		const bool ClearLogs,
		const bool ShowLogs,
		const bool ShowJson,
		const bool ClearAllAssets,
		const bool DontPackageAssets,
		const FString& PackageName,
		const FString& PackageDescription,
		const FString& PackageAuthor,
		const FString& PackagePath) :
			ClearLogs(ClearLogs),
			ShowLogs(ShowLogs),
			ShowJson(ShowJson),
			ClearAllAssets(ClearAllAssets),
			DontPackageAssets(DontPackageAssets),
			PackageName(PackageName),
			PackageDescription(PackageDescription),
			PackageAuthor(PackageAuthor),
			PackagePath(PackagePath){}
};
