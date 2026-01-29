#include "Utility/ArchiveUtility.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Model/Package/MPAssetRef.h"
#include "Serialization/Archive.h"

/**
 * Extract all the pkg files from the packages folder
 * @param InSearchPath - Package folder search path
 * @param InDestinationPath - Absolute destination path
  * @return true - Archives extracted successfully, false - Archives failed to extract
 */
bool FCPArchiveUtility::ExtractAllPackageArchives(const FString& InSearchPath, const FString& InDestinationPath)
{
	if (!FPaths::DirectoryExists(InSearchPath))
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Package Folder Not Found ]"));
		return false;
	}

	TArray<FString> PkgFiles;
	FindPackageArchiveFiles(InSearchPath, PkgFiles);

	if (PkgFiles.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ No Packages Found ]"));
		return false;
	}

	for (const FString& PkgFile : PkgFiles)
	{
		if (!ExtractPackageArchive(PkgFile, InDestinationPath))
		{
			UE_LOG(LogTemp, Error, TEXT("VWCP - [ Extract Package Archive Failed: %s ]"), *PkgFile);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("VWCP - [ Extracted Package: %s, Output Folder: %s ]"), *PkgFile, *InDestinationPath);
		}
	}

	return true;
}

/**
 * Find the package files in a search directory
 * @param SearchPath - Search path
 * @param OutPkgFiles - Array of package file paths
 */
void FCPArchiveUtility::FindPackageArchiveFiles(const FString& SearchPath, TArray<FString>& OutPkgFiles)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Lambda to filter .pkg files
	struct FPkgFileVisitor : public IPlatformFile::FDirectoryVisitor
	{
		TArray<FString>& FoundFiles;

		FPkgFileVisitor(TArray<FString>& InFiles) : FoundFiles(InFiles)
		{
		}

		virtual bool Visit(const TCHAR* FilenameOrDir, bool bIsDirectory) override
		{
			if (!bIsDirectory && FPaths::GetExtension(FilenameOrDir).Equals(TEXT("pkg"), ESearchCase::IgnoreCase))
			{
				FoundFiles.Add(FString(FilenameOrDir));
			}
			return true; // continue search
		}
	};

	FPkgFileVisitor Visitor(OutPkgFiles);
	PlatformFile.IterateDirectoryRecursively(*SearchPath, Visitor);
}

/**
 * Create an archive of absolute filenames
 * @param InFiles - File paths
 * @param InArchivePath - Absolute archive file path
 * @return true - success, false - failed
 */
bool FCPArchiveUtility::CreatePackageArchive(const TArray<FString>& InFiles, const FString& InArchivePath)
{
	TUniquePtr<FArchive> Archive(IFileManager::Get().CreateFileWriter(*InArchivePath));
	if (!Archive)
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Failed to create archive file: %s ]"), *InArchivePath);
		return false;
	}

	for (const FString& FilePath : InFiles)
	{
		UE_LOG(LogTemp, Log, TEXT("VWCP - [   ->  %s ]"), *FilePath);

		if (!FPaths::FileExists(FilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("VWCP - [ Archive file does not exist: %s ]"), *FilePath);
			return false;
		}

		FString ArchiveName = FilePath; // You can customize this to store relative paths
		*Archive << ArchiveName;

		TArray<uint8> FileData;
		if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("VWCP - [ Failed to load archive file into memory: %s ]"), *FilePath);
			return false;
		}

		int32 FileSize = FileData.Num();
		*Archive << FileSize;
		Archive->Serialize(FileData.GetData(), FileSize);
	}

	Archive->Close();
	return true;
}


/**
 * Extract the archive path into the destination path
 * @param InArchivePath - Absolute path of the archive
 * @param InDestinationPath - Absolute path where to extract the files
 * @return true - success, false - failed
 */
bool FCPArchiveUtility::ExtractPackageArchive(const FString& InArchivePath, const FString& InDestinationPath)
{
	TUniquePtr<FArchive> Archive(IFileManager::Get().CreateFileReader(*InArchivePath));
	if (!Archive)
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Failed to open archive file: %s ]"), *InArchivePath);
		return false;
	}

	while (!Archive->AtEnd())
	{
		FString FileName;
		*Archive << FileName;

		int32 FileSize = 0;
		*Archive << FileSize;

		if (FileSize <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("VWCP - [ Invalid archive file size for: %s ]"), *FileName);
			return false;
		}

		TArray<uint8> FileData;
		FileData.SetNumUninitialized(FileSize);
		Archive->Serialize(FileData.GetData(), FileSize);

		FString OutputPath = FPaths::Combine(InDestinationPath, FPaths::GetCleanFilename(FileName));
		if (!FFileHelper::SaveArrayToFile(FileData, *OutputPath))
		{
			UE_LOG(LogTemp, Error, TEXT("VWCP - [ Failed to write extracted archive file: %s ]"), *OutputPath);
			return false;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("VWCP - [ Archive Extracted: %s ]"), *OutputPath);
		}
	}

	Archive->Close();
	return true;
}
