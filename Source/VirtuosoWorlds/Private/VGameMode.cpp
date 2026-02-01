#include "Public/VGameMode.h"
#include "Misc/App.h"
#include "Misc/EngineVersion.h"
#include "Kismet/GameplayStatics.h"
#include "JsonObjectConverter.h"
#include "VPlayerState.h"
#include "Model/Land/VConfig.h"
#include "Model/Land/VServer.h"
#include "VDataService.h"
#include "VPlayerController.h"
#include "Public/Utility/VPackageArchiveUtility.h"
#include "Utility/VFileUtility.h"

AVGameMode::AVGameMode() : SpawnThresholdDistance(0)
{
	static ConstructorHelpers::FClassFinder<APawn> PawnClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")
	);

	if (PawnClass.Class)
	{
		DefaultPawnClass = PawnClass.Class;
	}

	PlayerControllerClass = AVPlayerController::StaticClass();
	PlayerStateClass = AVPlayerState::StaticClass();
}

/**
 * Initialize game entry
 * @param MapName - Level name
 * @param Options - Command line options
 * @param ErrorMessage - Error message
 */
void AVGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

/**
 * Start of the game mode
 * Dedicated Server and Listener Server
 */
void AVGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString ConfigFilename = TEXT("Server.json");
	FString ServerCMDValue;
	if (FParse::Value(FCommandLine::Get(), TEXT("setup="), ServerCMDValue))
		ConfigFilename = ServerCMDValue + TEXT(".json");

	FVConfig Config;
	FString ConfigPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / ConfigFilename);

	if (FString ConfigJSON; FFileHelper::LoadFileToString(ConfigJSON, *ConfigPath))
	{
		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FVConfig>(ConfigJSON, &Config, 0, 0))
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load the configuration: %s (Abort)"), *ConfigPath);
			ShutDownServer();
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load the configuration: %s (Abort)"), *ConfigPath);
		ShutDownServer();
		return;
	}

	FString CachePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Cache"));

	UE_LOG(LogTemp, Log, TEXT("============================================================================="));
	UE_LOG(LogTemp, Log, TEXT("V I R T U O S O   S E R V E R"));
	UE_LOG(LogTemp, Log, TEXT("UE Version: %s"), *FEngineVersion::Current().ToString());
	UE_LOG(LogTemp, Log, TEXT("Platform: %s"), FPlatformMisc::GetUBTPlatform());
	UE_LOG(LogTemp, Log, TEXT("Build Configuration: %s"), LexToString(FApp::GetBuildConfiguration()));
	UE_LOG(LogTemp, Log, TEXT("Build Target: %s"), LexToString(FApp::GetBuildTargetType()));
	UE_LOG(LogTemp, Log, TEXT("Server Mode: %s"), GetNetMode() == NM_DedicatedServer ? TEXT("Dedicated") : TEXT("Listen"));
	UE_LOG(LogTemp, Log, TEXT("Version: 1.0"));
	UE_LOG(LogTemp, Log, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("Options"));
	UE_LOG(LogTemp, Log, TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("Land Registry: %s"), Config.server.land_registry ? TEXT("Yes") : TEXT("No"));
	UE_LOG(LogTemp, Log, TEXT("Cache Path: %s"), *CachePath);
	UE_LOG(LogTemp, Log, TEXT("Empty Cache: %s"), Config.server.empty_cache ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Log, TEXT("Empty Database: %s"), Config.server.empty_db ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Log, TEXT("Package Imports: %s"), Config.server.package_imports ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Log, TEXT("CDN URL: %s"), *Config.server.cdn_url);
	UE_LOG(LogTemp, Log, TEXT("Spawn Radius: %f"), Config.server.spawn_radius);
	UE_LOG(LogTemp, Log, TEXT("============================================================================="));

	// Data Service
	UVDataService* DataService = UVDataService::Get(GetWorld());
	if (!DataService)
	{
		UE_LOG(LogTemp, Error, TEXT("Initialize - DataService Not Found (Abort)"));
		ShutDownServer();
		return;
	}

	if (!DataService->InitializeStore())
	{
		ShutDownServer();
		return;
	}

	// Empty cache?
	if (Config.server.empty_cache || Config.server.package_imports)
	{
		if (!FVFileUtility::EnsureEmptyDirectory(CachePath))
		{
			ShutDownServer();
			return;
		}
	}

	// Empty database?
	if (Config.server.empty_db || Config.server.package_imports)
	{
		if (!DataService->RemovalAll())
		{
			ShutDownServer();
			return;
		}
	}

	// Package Import (Pre-Load Server With World State)
	if (Config.server.package_imports)
	{
		if (FVPackageArchiveUtility::ExtractAllArchivesToCache())
		{
			TArray<FVMPackage> Packages;
			if (!FVPackageArchiveUtility::LoadPackagesFromCache(Packages))
				return;

			if (!DataService->AddPackages(Packages))
			{
				ShutDownServer();
				return;
			}
		}
		else
		{
			ShutDownServer();
			return;
		}
	}

}

/**
 * PreLogin user validation
 * @param Options - Login options 
 * @param Address - Remote client IP:Port
 * @param UniqueId - Unique login identifier
 * @param ErrorMessage - Error message (if set, will not call login)
 */
void AVGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// Check IP banlist

	// Check max players on server

	// Server validation
}

/**
 * Login User
 * @param NewPlayer - new player object
 * @param InRemoteRole - in remote role
 * @param Portal - portal
 * @param Options - url parameter options (e.g. username)
 * @param UniqueId - unique network identifier
 * @param ErrorMessage - error message
 * @return player controller instance
 */
APlayerController* AVGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

#if WITH_SERVER_CODE
	FString UserID = UGameplayStatics::ParseOption(Options, TEXT("user_id"));

	// Store user_id temporarily, keyed by UniqueNetId or Address
	PendingUserIDs.Add(UniqueId, UserID);
#endif
	return PC;
}

/**
 * Called after login and validation
 *
 * Replication snapshot has been sent to the client
 * 
 * @param NewPlayer - New player
 */
void AVGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		if (AVPlayerState* PState = NewPlayer->GetPlayerState<AVPlayerState>())
		{
			const FUniqueNetIdRepl NetId = NewPlayer->PlayerState->GetUniqueId();

			if (const FString* UserID = PendingUserIDs.Find(NetId))
			{
				PState->UserID = *UserID;

				PendingUserIDs.Remove(NetId);

				UE_LOG(LogTemp, Log, TEXT(
					       "PostLogin: Applied UserID=%s"),
				       **UserID
				);
			}
		}
	}
#endif
}


/**
 * Player logout
 * @param Exiting 
 */
void AVGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
#if WITH_SERVER_CODE
	UE_LOG(LogTemp, Log, TEXT("Game Mode - Logout [ Player Logged Out ]"));
#endif
}

/**
 * Spawn the player pawn
 * @param PC - Player controller
 * @param Position - Spawn position
 * @param Rotation - Spawn rotation
 */
void AVGameMode::SpawnPlayerPawn(APlayerController* PC, const FVector& Position, const FRotator& Rotation)
{
#if WITH_SERVER_CODE
	if (PC && PC->GetPawn() == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Mode - Spawn Player Pawn [ Position: %s, Rotation: %s ]"), *Position.ToString(), *Rotation.ToString());

		// Build a transform from your custom position/rotation
		FTransform SpawnTransform(Rotation, Position);

		// Respawn the player at this custom location
		RestartPlayerAtTransform(PC, SpawnTransform);
	}
#endif
}

/**
 * Shutdown the server
 */
void AVGameMode::ShutDownServer() const
{
	if (HasAuthority() && GetNetMode() != NM_Client)
	{
		// Option 1: Quit the game (works for both dedicated and listen servers)
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
	}
}
