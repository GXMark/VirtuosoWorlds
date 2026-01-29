#include "Utility/VFileUtility.h"
#include "Model/Constant/VConstants.h"
#include "HAL/FileManager.h"

// Empty a folder of its folders and files
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

#include "HAL/FileManager.h"
#include "Misc/Paths.h"

// Ensure that the directory is empty (will create the directory)
bool FVFileUtility::EnsureEmptyDirectory(const FString& AbsolutePath)
{
	IFileManager& FileManager = IFileManager::Get();

	FString Path = AbsolutePath;
	FPaths::NormalizeDirectoryName(Path);

	// Safety: must be absolute
	checkf(FPaths::IsRelative(Path) == false,
		TEXT("File Utility: Ensure Empty Directory requires an absolute path: %s"), *Path);

	// If directory exists, delete it completely
	if (FileManager.DirectoryExists(*Path))
	{
		if (!FileManager.DeleteDirectory(
			*Path,
			/* RequireExists */ false,
			/* Tree */ true
		))
		{
			UE_LOG(LogTemp, Error, TEXT("File Utility: Ensure Empty Directory: Failed to delete directory: %s"), *Path);
			return false;
		}
	}
	
	// Recreate directory (always)
	if (!FileManager.MakeDirectory(*Path, /* Tree */ true))
	{
		UE_LOG(LogTemp, Error, TEXT("File Utility: Ensure Empty Directory: Failed to create directory: %s"), *Path);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("File Utility: Ensure Empty Directory: Emptied and Created directory: %s"), *Path);
	return true;
}

// Load the supported asset as a byte array from file
bool FVFileUtility::LoadByteArrayFromFile(const FString& InAbsolutePath, const FGuid& InID, const FName& InType, TArray<uint8>& OutData)
{
	FString FilePath;

	if (InType == FVAssetType::Mesh)
		FilePath = FPaths::Combine(InAbsolutePath, InID.ToString() + TEXT(".mesh"));

	if (InType == FVAssetType::Texture)
		FilePath = FPaths::Combine(InAbsolutePath, InID.ToString() + TEXT(".texture"));

	if (FilePath.IsEmpty())
		return false;
	
	if (FPaths::FileExists(*FilePath))
		return FFileHelper::LoadFileToArray(OutData, *FilePath);

	return false;
}

// Save the asset as a byte array to a local file
bool FVFileUtility::SaveAssetAsByteArrayToFile(const FString& InAbsolutePath, const FName& InAssetType, const FGuid& InAssetID, const TArray<uint8>& InData)
{
	FString FilePath;

	if (InAssetType == FVAssetType::Mesh)
		FilePath = FPaths::Combine(InAbsolutePath, InAssetID.ToString() + TEXT(".mesh"));

	if (InAssetType == FVAssetType::Texture)
		FilePath = FPaths::Combine(InAbsolutePath, InAssetID.ToString() + TEXT(".texture"));

	if (!FilePath.IsEmpty())
		return FFileHelper::SaveArrayToFile(InData, *FilePath);

	return false;
}