#pragma once
#include "CoreMinimal.h"
#include "Model/Package/VMPackage.h"

/**
 * Package (Archive) Utility
 *
 * Features
 * 
 * Streams in chunks: avoids full memory loads
 * Scales to large files: no memory spikes
 * Load packages archives
 */
class VIRTUOSOWORLDS_API FVPackageArchiveUtility
{
public:
	static bool ExtractAllArchivesToCache();
	static bool LoadPackagesFromCache(TArray<FVMPackage>& OutDescriptors);
	static bool CreateArchive(const TArray<FString>& InFiles, const FString& InArchivePath);
	static bool ExtractArchive(const FString& InArchivePath, const FString& InDestinationPath);
private:
	static bool LoadPackage(const FString& InPackagePath, FVMPackage& OutPackage);
	static void FindFiles(const FString& InSearchDirectory, TArray<FString>& OutFilePaths, const FString& InExtension);
};
