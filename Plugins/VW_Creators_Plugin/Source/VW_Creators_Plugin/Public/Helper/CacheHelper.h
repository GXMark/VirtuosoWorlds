#pragma once
#include "CoreMinimal.h"
#include "Model/Package/MPAssetRef.h"

class VW_CREATORS_PLUGIN_API FPCacheHelper
{
public:
	static bool Initialize(const FString& InAssetFolderPath, bool ClearAll, bool ShowLogs);

	// File management
	static bool SetAssetFolder(const FString& InPath);
	static FString GetAssetFolder();
	static bool AssetExists(const FGuid& InAssetID, const FName& InAssetType);
	static bool SavePackage(const FGuid& InPackageID, const FString& InJSON);
	static bool LoadPackage(const FString& InPackageName, FString& OutJSON);
	static bool LoadAssetFromFile(const FGuid& InAssetID, const FName& InAssetType, TArray<uint8>& OutData);
	static bool SaveTextureAsset(const FGuid& InAssetID, const TArray<uint8>& InData);
	static bool SetupAssetFolder(const bool InClearAll);
	
	static void AddAssetRef(const FMPAssetRef& InAssetRef);
	static TArray<FMPAssetRef> GetAssetRefs();
	static TArray<FMPAssetRef> GetAssetRefsByType(const FName& InAssetType);
	static TArray<FString> GetAssetPaths();

	
	// Asset registry lookup
	static UTexture2D* LoadTextureFromAssetRegistry(const FString& InAssetPath);
	static UStaticMesh* LoadMeshFromAssetRegistry(const FString& InAssetPath);
	
private:
	static bool ShowLogs;
	static FString AssetFolder;
	static TArray<FMPAssetRef> AssetRefs;
	static TArray<FGuid> AssetIDs;
};
