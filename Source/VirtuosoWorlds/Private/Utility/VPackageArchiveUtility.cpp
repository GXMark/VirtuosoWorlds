#include "Public/Utility/VPackageArchiveUtility.h"

#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Model/Package/VMPackage.h"
#include "Serialization/Archive.h"

/**
 * Extract all the parchive files from the packages folder to the cache folder
 * @return true - archives extracted successfully, false - archive extraction failed
 */
bool FVPackageArchiveUtility::ExtractAllArchivesToCache()
{
	FString PackageArchivePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Packages"));
	FString CachePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Cache"));
	
	if (!FPaths::DirectoryExists(PackageArchivePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Extract All Archives [ Search Folder Not Found ]"));
		return false;
	}

	TArray<FString> PArchiveFiles;
	FindFiles(PackageArchivePath, PArchiveFiles, TEXT(".parchive"));

	if (PArchiveFiles.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Extract All Archives [ No Packages Found ]"));
		return false;
	}

	for (const FString& PArchive : PArchiveFiles)
	{
		if (!ExtractArchive(PArchive, CachePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Extract All Archives [ Import Package Failed Decryption: %s ]"), *PArchive);
			return false;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Package Archive Utility - Extract All Archives [ Packages: %d ]"), PArchiveFiles.Num());

	return true;
}

/**
 * Load all packages from the packages folder
 * @param Packages - Array of packages
 * @return true - packages loaded successfully, false - packages not loaded
 */
bool FVPackageArchiveUtility::LoadPackagesFromCache(TArray<FVMPackage>& Packages)
{
	FString CachePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Cache"));

	TArray<FString> PackageFiles;
	FindFiles(CachePath, PackageFiles, TEXT(".package"));

	if (PackageFiles.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Load Packages [ No Packages Found ]"));
		return false;
	}

	for (const FString& File : PackageFiles)
	{
		FVMPackage Package;
		
		FString PackageFilePath = FPaths::Combine(CachePath, FPaths::GetBaseFilename(File) + TEXT(".package"));
		
		if (!LoadPackage(PackageFilePath, Package))
		{
			UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Load Packages [ Package Failed: %s ]"), *File);
			return false;
		}

		Packages.Add(Package);

		UE_LOG(LogTemp, Log, TEXT("Package Archive Utility - Load Package [ %s ]"), *FPaths::GetCleanFilename(File));
	}

	return true;
}

/**
 * Load Package
 * @param InPackagePath - Package absolute file path
 * @param OutPackage - Package model
 * @return true - Package loaded, false - Package not loaded
 */
bool FVPackageArchiveUtility::LoadPackage(const FString& InPackagePath, FVMPackage& OutPackage)
{
	FString OutPackageJSON = TEXT("");
	
	if (!InPackagePath.IsEmpty())
	{
		FFileHelper::LoadFileToString(OutPackageJSON, *InPackagePath);
	
		// Deserialize the package
		return FJsonObjectConverter::JsonObjectStringToUStruct(OutPackageJSON, &OutPackage, 0, 0);
	}

	return false;
}

/*/**
 * Load the package from the file
 * @param InName - Package name
 * @param OutData - String
 * @return true - success, false - failed
 #1#
bool FPackageArchiveUtility::LoadPackageFromFile(const FString& InName, FString& OutData)
{
	FString FilePath = FPaths::Combine(CachePath, InName + TEXT(".package"));

	if (!FilePath.IsEmpty())
		return FFileHelper::LoadFileToString(OutData, *FilePath);

	return true;
}*/

/**
 * Find the files in a search directory with a given extension
 * @param SearchPath - Search path
 * @param OutFilePaths - Array of file paths
 * @param InExtension - File extension
 */
void FVPackageArchiveUtility::FindFiles(const FString& SearchPath, TArray<FString>& OutFilePaths, const FString& InExtension)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Lambda to filter .pkg files
	struct FFileVisitor : public IPlatformFile::FDirectoryVisitor
	{
		TArray<FString>& FoundFiles;
		FString WantedExtension;

		FFileVisitor(TArray<FString>& InFiles, const FString& InExtension)
			: FoundFiles(InFiles)
			  , WantedExtension(InExtension)
		{
		}

		virtual bool Visit(const TCHAR* FilenameOrDir, bool bIsDirectory) override
		{
			if (!bIsDirectory && FPaths::GetExtension(FilenameOrDir, true).Equals(WantedExtension, ESearchCase::IgnoreCase))
			{
				FoundFiles.Add(FString(FilenameOrDir));
			}
			return true; // continue search
		}
	};

	FFileVisitor Visitor(OutFilePaths, InExtension);
	PlatformFile.IterateDirectoryRecursively(*SearchPath, Visitor);
}

/**
 * Create an archive of absolute filenames
 * @param InFiles - Absolute file paths
 * @param InArchivePath - Absolute archive file path
 * @return true - success, false - failed
 */
bool FVPackageArchiveUtility::CreateArchive(const TArray<FString>& InFiles, const FString& InArchivePath)
{
	TUniquePtr<FArchive> Archive(IFileManager::Get().CreateFileWriter(*InArchivePath));
	if (!Archive)
	{
		UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Create Archive [ Failed To Create File: %s ]"), *InArchivePath);
		return false;
	}

	for (const FString& FilePath : InFiles)
	{
		if (!FPaths::FileExists(FilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Create Archive [ File Does Not Exist: %s ]"), *FilePath);
			return false;
		}

		FString ArchiveName = FilePath; // You can customize this to store relative paths
		*Archive << ArchiveName;

		TArray<uint8> FileData;
		if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - Create Archive [ Failed To Load File Into Memory: %s ]"), *FilePath);
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
bool FVPackageArchiveUtility::ExtractArchive(const FString& InArchivePath, const FString& InDestinationPath)
{
	TUniquePtr<FArchive> Archive(IFileManager::Get().CreateFileReader(*InArchivePath));
	if (!Archive)
	{
		UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - [ Archive Failed To Open File: %s ]]"), *InArchivePath);
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
			UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - [ Archive Invalid File Size For: %s ]"), *FileName);
			return false;
		}

		TArray<uint8> FileData;
		FileData.SetNumUninitialized(FileSize);
		Archive->Serialize(FileData.GetData(), FileSize);


		FString OutputPath = FPaths::Combine(InDestinationPath, FPaths::GetCleanFilename(FileName));
		if (!FFileHelper::SaveArrayToFile(FileData, *OutputPath))
		{
			UE_LOG(LogTemp, Error, TEXT("Package Archive Utility - [ Archive Failed To Write Extracted File: %s ]"), *OutputPath);
			return false;
		}
	}

	Archive->Close();
	return true;
}
