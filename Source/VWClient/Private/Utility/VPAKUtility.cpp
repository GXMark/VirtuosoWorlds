#include "Utility/VPAKUtility.h"
#include "IPlatformFilePak.h"

// Register the PAK platform - not used now
bool FVPAKUtility::Register()
{
	if (!FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile")))
	{
		FPakPlatformFile* PakPlatform = new FPakPlatformFile();
		if (PakPlatform->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT("")))
		{
			FPlatformFileManager::Get().SetPlatformFile(*PakPlatform);
			return true;
		}

		// Handle init failure
		delete PakPlatform;
		return false;
	}

	return true;
}

// Mount PAK
bool FVPAKUtility::Mount(const FString& InPAKPath, const FString& InAssetID)
{
	// Find the existing Pak platform file (do NOT create a new platform file)
	if (FPakPlatformFile* PakPlatform = static_cast<FPakPlatformFile*>(FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"))))
	{
		const FString MountPoint = FPaths::ProjectContentDir() + InAssetID + TEXT("/");
				
		//UE_LOG(LogTemp, Log, TEXT("Mounting PAKPath: %s, MountPoint: %s"), *InPAKPath, *MountPoint);
		return PakPlatform->Mount(*InPAKPath, 0, *MountPoint);
	}

	UE_LOG(LogTemp, Error, TEXT("Asset Loader - Mount [ PakPlatformFile not found. Engine may not be using PakFile platform. ]"));
	return false;
}