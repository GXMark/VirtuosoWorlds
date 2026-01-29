#include "Utility/PAKUtility.h"
#include "Constant/Constants.h"
#include "Helper/CacheHelper.h"

void FCPPAKUtility::CookCurrentLevel()
{
	// Path to RunUAT.bat
	FString UATPath = FPaths::ConvertRelativePathToFull(FPaths::EngineDir() / TEXT("Build/BatchFiles/RunUAT.bat"));

	
	FString ProjectFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
	FString CurrentLevel = GEditor->GetEditorWorldContext().World()->GetCurrentLevel()->GetOutermost()->GetName();
	
	// Command-line arguments
	FString CmdArgs = FString::Printf(TEXT("BuildCookRun -project=\"%s\" -noP4 -target=VirtuosoWorlds -platform=Win64 -clientconfig=Development -cook -skipstage -nopak -nostrict -map=%s"),*ProjectFilePath,*CurrentLevel);
	
	// Create the process
	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;
	FPlatformProcess::CreatePipe(ReadPipe, WritePipe);

	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*UATPath,
		*CmdArgs,
		true,    // bLaunchDetached
		false,   // bLaunchHidden
		false,   // bLaunchReallyHidden
		nullptr, // OutProcessID
		0,       // PriorityModifier
		nullptr, // OptionalWorkingDirectory
		WritePipe
	);

	if (ProcHandle.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("VWCP - Cook Current Level Successfully"));

		// Wait for it to complete (optional)
		FPlatformProcess::WaitForProc(ProcHandle);

		// Close process handle
		FPlatformProcess::CloseProc(ProcHandle);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch UAT"));
	}
}


// Create PAK from unreal game assets
void FCPPAKUtility::CreatePAK(const FGuid& InAssetID, const FName& InAssetType, const FName& InAssetName, const FName& InAssetPath)
{
	// Get project-name	
	FString ProjectFilePath = FPaths::GetProjectFilePath();
	FString ProjectName = FPaths::GetBaseFilename(ProjectFilePath);

	// Create absolute paths
	FString EngineDir = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	FString PakToolPath = FPaths::Combine(EngineDir, TEXT("Binaries/Win64/UnrealPak.exe"));
	
	FString PakOutputPath;

	// Supported pak asset types
	if (InAssetType == FPAssetType::Mesh)
		PakOutputPath = FPaths::Combine(FPCacheHelper::GetAssetFolder(), InAssetID.ToString() + TEXT(".mesh"));

	if (PakOutputPath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - PAK Utility [ Asset type not supported for: %s, Name: %s]"), *InAssetID.ToString(), *InAssetName.ToString());
		return;
	}
	
	FString ResponseFilePath = FPaths::Combine(ProjectDir, TEXT("MyPakList.txt"));

	FString AssetPath = PrepareAssetPath(InAssetPath);

	if (AssetPath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - PAK Utility [ Asset path invalid: %s ]"), *AssetPath);
		return;
	}

	// Create the PAK list content (it's overwritten because we don't need to store it)
	FString AssetPath1 = ProjectDir + "Saved/Cooked/Windows/" + ProjectName + "/" + AssetPath + TEXT(".uasset");
	FString AssetPath2 = ProjectDir + "Saved/Cooked/Windows/" + ProjectName + "/" + AssetPath + TEXT(".uexp");
	FString AssetPath3 = ProjectDir + "Saved/Cooked/Windows/" + ProjectName + "/" + AssetPath + TEXT(".ubulk");
	
	AssetPath.RemoveFromStart("Content/");

	// Mount path (does not require the asset paths to resolve in loader)
	FString MountPath1 = AssetPath + TEXT("/") + InAssetName.ToString() + TEXT(".uasset");
	FString MountPath2 = AssetPath + TEXT("/") + InAssetName.ToString() + TEXT(".uexp");
	FString MountPath3 = AssetPath + TEXT("/") + InAssetName.ToString() + TEXT(".ubulk");
		
	FString ResponseContent;

	if (FPaths::FileExists(AssetPath1))
	{
		ResponseContent = FString::Printf(TEXT("\"%s\" \"%s\"\n"), *AssetPath1, *MountPath1);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - PAK Utility [ Cooked asset does not exist: %s - Perform a Re-Cook ]"), *AssetPath1);
		return;
	}

	// Some cooked assets have a uexp and ubulk
	if (FPaths::FileExists(AssetPath2))
		ResponseContent += FString::Printf(TEXT("\"%s\" \"%s\"\n"), *AssetPath2, *MountPath2);

	if (FPaths::FileExists(AssetPath3))
		ResponseContent += FString::Printf(TEXT("\"%s\" \"%s\"\n"), *AssetPath3, *MountPath3);

	
	FFileHelper::SaveStringToFile(ResponseContent, *ResponseFilePath);

	FString CommandLine = FString::Printf(TEXT("\"%s\" -create=\"%s\" -compress -compressionformats=oodle"), *PakOutputPath, *ResponseFilePath);
	
	//UE_LOG(LogTemp, Log, TEXT("PAK Utility - [ CommandLine: %s ]"), *CommandLine);
	
	// Launch UnrealPak
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*PakToolPath,
		*CommandLine,
		true, // bLaunchDetached
		false, // bLaunchHidden
		false, // bLaunchReallyHidden
		nullptr,
		0,
		nullptr,
		nullptr
	);

	if (ProcHandle.IsValid())
	{
		// Wait until the pak has finished processing
		while (FPlatformProcess::IsProcRunning(ProcHandle))
		{
		}

		UE_LOG(LogTemp, Log, TEXT("VWCP - [ PAK created, Asset Path: %s, Asset ID: %s ]"), *AssetPath, *InAssetID.ToString());
	}
}

/**
 * Prepare the asset path
 * @param InAssetPath - Asset path
 * @return true - prepared successfully, false - not prepared successfully
 */
FString FCPPAKUtility::PrepareAssetPath(const FName& InAssetPath)
{
	if (!InAssetPath.IsValid())
		return FString();

	FString AssetPath = InAssetPath.ToString();
	
	// Replace the Game with Content
	AssetPath.ReplaceInline(TEXT("/Game"), TEXT("Content"));

	// Strip everything after the period
	if (int32 DotIndex = 0; AssetPath.FindChar('.', DotIndex))
		AssetPath = AssetPath.Left(DotIndex);
	else
		return FString();

	return AssetPath;
}


/**
 * Create the PAK asset load path from the original source asset path from editor
 *
 * PAK Load Path Format: /<MountPoint>/<Path>/<AssetName.AssetName>
 * 
 * @param InMountPoint - Mount point to prefix for loading the PAK
 * @param InAssetSourcePath - Original asset source path in the editor 
 * @param OutPAKLoadPath - PAK Asset Load Path
 * @return true - created successfully, false - failed to create
 */
bool FCPPAKUtility::CreatePAKAssetLoadPath(const FString& InMountPoint, const FString& InAssetSourcePath, FString& OutPAKLoadPath)
{
	// Make a copy
	OutPAKLoadPath = InAssetSourcePath;
	
	// Remove the Game prefix
	if (!OutPAKLoadPath.StartsWith(TEXT("/Game")))
		return false;
	
	OutPAKLoadPath.RemoveFromStart(TEXT("/Game"));
	
	// Prefix the mount point
	OutPAKLoadPath = FString::Printf(TEXT("%s%s"), *InMountPoint, *OutPAKLoadPath);

	return true;
}
