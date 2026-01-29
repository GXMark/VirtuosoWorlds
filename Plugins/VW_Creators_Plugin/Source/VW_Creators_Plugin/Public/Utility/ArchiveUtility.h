#pragma once
#include "CoreMinimal.h"

/**
 * Archive Utility
 *
 * Streams in chunks: avoids full memory loads
 * Scales to large files: no memory spikes
 */
class VW_CREATORS_PLUGIN_API FCPArchiveUtility
{
public:
	static bool ExtractAllPackageArchives(const FString& InSearchPath, const FString& InDestinationPath);
	static bool CreatePackageArchive(const TArray<FString>& InFiles, const FString& InArchivePath);
	static bool ExtractPackageArchive(const FString& InArchivePath, const FString& InDestinationPath);
private:
	static void FindPackageArchiveFiles(const FString& InSearchDirectory, TArray<FString>& OutPkgFiles);
};
