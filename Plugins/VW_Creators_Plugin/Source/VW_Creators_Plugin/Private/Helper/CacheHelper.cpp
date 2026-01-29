#include "Helper/CacheHelper.h"
#include "Constant/Constants.h"

// Initialize
bool FPCacheHelper::ShowLogs = false;
FString FPCacheHelper::AssetFolder = FString();
TArray<FMPAssetRef> FPCacheHelper::AssetRefs = {};
TArray<FGuid> FPCacheHelper::AssetIDs = {};

// Initialize the asset cache
bool FPCacheHelper::Initialize(const FString& InAssetFolderPath, bool ClearAll, bool ShowOutputLogs)
{
	AssetFolder = InAssetFolderPath;
	AssetRefs.Empty();
	AssetIDs.Empty();

	if (!SetupAssetFolder(ClearAll))
	{
		UE_LOG(LogTemp, Error, TEXT("Cache Helper - [ Status: Asset folder creation error: %s ]"), *InAssetFolderPath);
		return false;
	}

	ShowLogs = ShowOutputLogs;

	return true;
}

/**
 * Set Asset Folder
 * @param InPath - Asset absolute folder path
 */
bool FPCacheHelper::SetAssetFolder(const FString& InPath)
{
	IFileManager& FileManager = IFileManager::Get();

	if (InPath.IsEmpty())
		return false;

	if (FileManager.DirectoryExists(*InPath))
	{
		AssetFolder = InPath;
		return true;
	}

	return false;
}

/**
 * Get Asset Folder
 * @return Asset absolute folder path
 */
FString FPCacheHelper::GetAssetFolder()
{
	return AssetFolder;
}

/**
 * Does the asset file exist in the asset folder?
 * @param InAssetID - Asset identifier
 * @param InAssetType - Asset type
 * @return true - Cached, false - Not Cached
 */
bool FPCacheHelper::AssetExists(const FGuid& InAssetID, const FName& InAssetType)
{
	FString AssetFilePath;

	if (InAssetType == FPAssetType::Package)
		AssetFilePath = FString::Printf(TEXT("%s/%s.package"), *AssetFolder, *InAssetID.ToString());

	if (InAssetType == FPAssetType::Mesh)
		AssetFilePath = FString::Printf(TEXT("%s/%s.mesh"), *AssetFolder, *InAssetID.ToString());

	if (InAssetType == FPAssetType::Texture)
		AssetFilePath = FString::Printf(TEXT("%s/%s.texture"), *AssetFolder, *InAssetID.ToString());

	if (!AssetFilePath.IsEmpty())
	{
		IFileManager& FileManager = IFileManager::Get();
		return FileManager.FileExists(*AssetFilePath);
	}

	return false;
}


/**
 * Set up the asset folder with an option to clear all assets
 * 
 * @param InClearAll - Clear all the assets
 * @return true - setup successful, false - setup failed
 */
bool FPCacheHelper::SetupAssetFolder(const bool InClearAll)
{
	IFileManager& FileManager = IFileManager::Get();

	// Asset folder path empty
	if (AssetFolder.IsEmpty())
		return false;

	const bool Exists = FileManager.DirectoryExists(*AssetFolder);

	// Does the asset folder exist with the clear-all flag?
	if (Exists && InClearAll)
	{
		FileManager.DeleteDirectory(*AssetFolder, true, true);
		return FileManager.MakeDirectory(*AssetFolder, true);
	}

	// Create an asset folder if not exists
	if (!Exists)
		return FileManager.MakeDirectory(*AssetFolder, true);

	// The asset folder already exists, do nothing
	return true;
}

// Add asset ref
void FPCacheHelper::AddAssetRef(const FMPAssetRef& InAssetRef)
{
	if (!AssetIDs.Contains(InAssetRef.id))
	{
		AssetRefs.Add(InAssetRef);
		AssetIDs.Add(InAssetRef.id);
	}
}

// Get asset ref array
TArray<FMPAssetRef> FPCacheHelper::GetAssetRefs()
{
	return AssetRefs;
}

TArray<FMPAssetRef> FPCacheHelper::GetAssetRefsByType(const FName& InAssetType)
{
	TArray<FMPAssetRef> Result;
	
	for (FMPAssetRef AssetRef : AssetRefs)
	{
		if (AssetRef.type == InAssetType)
			Result.Add(AssetRef);
	}
	return Result;
}


/**
 * Get asset paths
 * @return Asset paths
 */
TArray<FString> FPCacheHelper::GetAssetPaths()
{
	TArray<FString> AssetPaths;

	for (FMPAssetRef AssetRef : AssetRefs)
	{
		if (AssetRef.type == FPAssetType::Package)
			AssetPaths.Add(FPaths::Combine(AssetFolder, AssetRef.id.ToString() + TEXT(".package")));
		else if (AssetRef.type == FPAssetType::Mesh)
			AssetPaths.Add(FPaths::Combine(AssetFolder, AssetRef.id.ToString() + TEXT(".mesh")));
		else if (AssetRef.type == FPAssetType::Texture)
			AssetPaths.Add(FPaths::Combine(AssetFolder, AssetRef.id.ToString() + TEXT(".texture")));
		else
			UE_LOG(LogTemp, Error, TEXT("Asset Service - [ Status: Unsupported Asset: %s ]"), *AssetRef.id.ToString());
	}

	return AssetPaths;
}

/**
 * Save texture asset to the asset folder
 * @param InAssetID - Asset identifier
 * @param InData - Byte array data
 * @return Success - true, Failed - false
 */
bool FPCacheHelper::SaveTextureAsset(const FGuid& InAssetID, const TArray<uint8>& InData)
{
	FString FilePath = FString::Printf(TEXT("%s/%s.texture"), *AssetFolder, *InAssetID.ToString());
	return FFileHelper::SaveArrayToFile(InData, *FilePath);
}

/**
 * Save package to the asset folder
 * @param InPackageID - Package identifier
 * @param InJSON - Package JSON String
 * @return true - Success, false - Failed
 */
bool FPCacheHelper::SavePackage(const FGuid& InPackageID, const FString& InJSON)
{
	FString FilePath = FString::Printf(TEXT("%s/%s.package"), *AssetFolder, *InPackageID.ToString());
	return FFileHelper::SaveStringToFile(InJSON, *FilePath);
}

/**
 * Load package from the asset folder
 * @param InPackageName - Package name
 * @param OutJSON - Package JSON string
 * @return true - Success, false - Failed
 */
bool FPCacheHelper::LoadPackage(const FString& InPackageName, FString& OutJSON)
{
	FString FilePath = FString::Printf(TEXT("%s/%s.package"), *AssetFolder, *InPackageName);
	return FFileHelper::LoadFileToString(OutJSON, *FilePath);
}

/**
 * Load asset from the assets folder
 * @param InAssetID - Asset identifier
 * @param InAssetType - Asset type
 * @param OutData - Raw byte array
 * @return true - load success, false - load failed
 */
bool FPCacheHelper::LoadAssetFromFile(const FGuid& InAssetID, const FName& InAssetType, TArray<uint8>& OutData)
{
	FString FilePath = TEXT("");
	if (InAssetType == FPAssetType::Mesh)
		FilePath = FString::Printf(TEXT("%s/%s.mesh"), *AssetFolder, *InAssetID.ToString());
	else if (InAssetType == FPAssetType::Texture)
		FilePath = FString::Printf(TEXT("%s/%s.texture"), *AssetFolder, *InAssetID.ToString());
	if (!FilePath.IsEmpty())
		return FFileHelper::LoadFileToArray(OutData, *FilePath);

	return false;
}

/**
 * Find texture in asset registry
 * @param InAssetPath - Asset path
 * @return Texture
 */
UTexture2D* FPCacheHelper::LoadTextureFromAssetRegistry(const FString& InAssetPath)
{
	if (UTexture2D* LoadedTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *InAssetPath)))
		return LoadedTexture;

	return nullptr;
}

/**
 * Find mesh in asset registry
 * @param InAssetPath - Asset path
 * @return Mesh
 */
UStaticMesh* FPCacheHelper::LoadMeshFromAssetRegistry(const FString& InAssetPath)
{
	if (UStaticMesh* LoadedMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *InAssetPath)))
		return LoadedMesh;

	return nullptr;
}
