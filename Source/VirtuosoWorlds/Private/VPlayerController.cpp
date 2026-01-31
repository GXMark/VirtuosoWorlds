#include "Public/VPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include <EnhancedInputComponent.h>
#include "Blueprint/UserWidget.h"
#include "RHI.h"
#include "RendererInterface.h"
#include "VPlayerState.h"
#include "JsonObjectConverter.h"

#if WITH_SERVER_CODE
#include "VDataService.h"
#include "Region/VRegionServerBridge.h"
#endif

#if WITH_CLIENT_CODE
#include "Subsystem/VAssetManager.h"
#include "Subsystem/VRegionClientSubsystem.h"
#include "Region/VRegionClientBridge.h"
#endif

AVPlayerController::AVPlayerController()
{
	// Client streaming is handled by the region client subsystem.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.f;
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBP(TEXT("/Game/UI/WBP_Region"));
	if (WidgetBP.Succeeded())
	{
		GameWidgetClass = WidgetBP.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Default_Finder(
		TEXT("/Game/Input/IMC_Default.IMC_Default")
	);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MouseLook_Finder(
		TEXT("/Game/Input/IMC_MouseLook.IMC_MouseLook")
	);

	if (IMC_Default_Finder.Succeeded())
	{
		IMC_Default = IMC_Default_Finder.Object;
	}
		
	if (IMC_MouseLook_Finder.Succeeded())
	{
		IMC_MouseLook = IMC_MouseLook_Finder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftClickActionAsset(
		TEXT("/Game/Input/Actions/IA_LeftClick.IA_LeftClick"));

	if (LeftClickActionAsset.Succeeded())
	{
		LeftClickAction = LeftClickActionAsset.Object;
	}
}

#if WITH_CLIENT_CODE
void AVPlayerController::ClientRequestSpatialItems(const FVector& Origin, int32 MaxItems)
{
	// Client-driven spatial item requests are disabled; server pushes via replication only.
	UE_LOG(LogTemp, Verbose, TEXT("PlayerController: ClientRequestSpatialItems ignored (server-authoritative)."));
	(void)Origin;
	(void)MaxItems;
}

void AVPlayerController::ClientRequestMaterialsBatch(const TArray<FGuid>& MaterialIds)
{
	ServerRequestMaterialsBatch(MaterialIds);
}
#endif

void AVPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AVPlayerState* PState = GetPlayerState<AVPlayerState>();
	if (!PState)
		return;

	UE_LOG(LogTemp, Log, TEXT(
		       "Client received UserID=%s"),
	       *PState->UserID
	);

#if WITH_CLIENT_CODE
	MaxSpatialDistance = PState->MaxSpatialDistance;
	SpatialItemsReceived = 0;
	bSpawnedAtSpatialThreshold = false;
	UE_LOG(LogTemp, Log, TEXT("Client received MaxSpatialDistance=%.2f"), MaxSpatialDistance);
#endif
}

void AVPlayerController::BeginPlay()
{
	Super::BeginPlay();

#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		RegionServerBridge = NewObject<URegionServerBridge>(this);
		RegionServerBridge->Initialize(GetWorld());
	}
#endif

	// Only allow clients beyond this point
	if (GetNetMode() != NM_Client)
		return;

	if (IsLocalController())
	{
#if WITH_CLIENT_CODE
		UE_LOG(LogTemp, Log, TEXT("============================================================================="));
		UE_LOG(LogTemp, Log, TEXT("V I R T U O S O   C L I E N T"));
		UE_LOG(LogTemp, Log, TEXT("============================================================================="));
		
		UVAssetManager* AssetManager = UVAssetManager::Get(GetWorld());
		if (!AssetManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Player Controller - Begin Play [ Cache Manager Not Found ]"));
		}
		else
		{
			FString CachePath = FPaths::ProjectSavedDir() + TEXT("Cache");
			if (!AssetManager->Initialize(CachePath, false, TEXT("https://www.virtuosoworlds.com"), TEXT("tictactoe"), false))
			{
				UE_LOG(LogTemp, Error, TEXT("Player Controller - Begin Play [ Cache Manager Failed To Initialize ]"));
			}
		}

#endif
	}
}


void AVPlayerController::InitPlayerState()
{
	// Spawn the correct PlayerState
	Super::InitPlayerState();

	UE_LOG(LogTemp, Log, TEXT("Player Controller - Initialize Player State"));
}

void AVPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Log, TEXT("Player Controller - End Play"));
}

/**
 * Get the client player controller
 * @param WorldContext - World context
 * @return Player controller
 */
AVPlayerController* AVPlayerController::Get(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;

	if (UWorld* World = WorldContext->GetWorld())
	{
		return Cast<AVPlayerController>(World->GetFirstPlayerController());
	}
	return nullptr;
}

/**
 * Set input component
 */
void AVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogTemp, Log, TEXT("Initialize player controller for local player"));

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(IMC_Default, 0);
		Subsystem->AddMappingContext(IMC_MouseLook, 1);


		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
		{
			Input->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AVPlayerController::OnLeftMouseDown);
			Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AVPlayerController::OnLeftMouseUp);
		}
	}

	/*// Add Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}

		//UE_LOG(LogTemp, Log, TEXT("Bind left mouse button click action"));
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
		{
			Input->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AVPlayerController::OnLeftMouseDown);
			Input->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AVPlayerController::OnLeftMouseUp);
		}
	}*/
}

/**
 * On player pawn possessed
 * @param InPawn - Player pawn
 */
void AVPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("Player Controller - On Possess [ Status: Possess Player ]"));
}

// Ignore the look input when the left mouse is held down
void AVPlayerController::OnLeftMouseDown()
{
	SetIgnoreLookInput(true);
}

// Reset the look input to active when the left mouse button is released
void AVPlayerController::OnLeftMouseUp()
{
	ResetIgnoreLookInput();
}

// EVENT LISTENERS

void AVPlayerController::OnSpawnPlayer() const
{
#if WITH_CLIENT_CODE
	if (IsLocalController())
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller - On Spawn Player"));
		//ServerSpawnPlayer();
	}
#endif
}


// RPCs

/**
 * Server spawn player
 */
void AVPlayerController::ServerSpawnPlayer_Implementation()
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		/*if (const AMyPlayerState* PS = Cast<AMyPlayerState>(this->PlayerState))
		{
			if (AVirtuosoWorldsGameMode* GM = Cast<AVirtuosoWorldsGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->SpawnPlayerPawn(this, FVector(PS->Parcel.landing_position), FRotator(PS->Parcel.landing_rotation.X, PS->Parcel.landing_rotation.Z, PS->Parcel.landing_rotation.Y));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Player Controller - [RPC] Request Spawn Pawn [ Status: Game mode not found ]"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Player Controller - [RPC] Request Spawn Pawn [ Status: Player state not found ]"));
		}*/
	}
#endif
}

/**
 * Server Region State
 * 
 * Region state data sent from client to server
 * 
 * Note: Currently no data type defined
 */
void AVPlayerController::ServerRegionState_Implementation()
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Player Controller - [RPC] Server Region State"));

		// Access Region State
	}
#endif
}


// =========================
// Spatial streaming RPCs
// =========================

#if WITH_SERVER_CODE
void AVPlayerController::ServerSendSpatialItems(const TArray<FVMSpatialItemNet>& Items, bool bHasMore)
{
	// Send down to the owning client via existing RPC.
	ClientReceiveSpatialItems(Items, bHasMore);
}

void AVPlayerController::ServerSendMaterialsBatch(const TArray<FVMMaterial>& Materials)
{
	ClientReceiveMaterialsBatch(Materials);
}
#endif

void AVPlayerController::ServerRequestSpatialItems_Implementation(const FVector& Origin, int32 MaxItems)
{
#if WITH_SERVER_CODE
	// Client-driven spatial item requests are disabled; server pushes via replication only.
	UE_LOG(LogTemp, Verbose, TEXT("PlayerController: ServerRequestSpatialItems ignored (server-authoritative)."));
	(void)Origin;
	(void)MaxItems;
#else
	ClientReceiveSpatialItems(TArray<FVMSpatialItemNet>(), false);
#endif
}

void AVPlayerController::ClientReceiveSpatialItems_Implementation(const TArray<FVMSpatialItemNet>& Items, bool bHasMore)
{
#if WITH_CLIENT_CODE
	SpatialItemsReceived += Items.Num();
	if (!bSpawnedAtSpatialThreshold && MaxSpatialDistance > 0.f
		&& SpatialItemsReceived >= MaxSpatialDistance && IsLocalController())
	{
		bSpawnedAtSpatialThreshold = true;
		ServerSpawnPlayer();
	}

	// Forward to the region client presenter.
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
#endif
}

float AVPlayerController::GetSpatialDistancePercent() const
{
#if WITH_CLIENT_CODE
	if (MaxSpatialDistance <= 0.f)
	{
		return 0.f;
	}

	const float Percent = (static_cast<float>(SpatialItemsReceived) / MaxSpatialDistance) * 100.f;
	return FMath::Clamp(Percent, 0.f, 100.f);
#else
	return 0.f;
#endif
}

// =========================
// Material RPCs
// =========================

void AVPlayerController::ServerRequestMaterialsBatch_Implementation(const TArray<FGuid>& MaterialIds)
{
#if WITH_SERVER_CODE
	if (!HasAuthority())
	{
		return;
	}

	if (!RegionServerBridge)
	{
		RegionServerBridge = NewObject<URegionServerBridge>(this);
		RegionServerBridge->Initialize(GetWorld());
	}

	RegionServerBridge->HandleMaterialsRequest(this, MaterialIds);
#endif
}

void AVPlayerController::ClientReceiveMaterialsBatch_Implementation(const TArray<FVMMaterial>& Materials)
{
#if WITH_CLIENT_CODE
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (URegionClientSubsystem* RegionSubsystem = World->GetSubsystem<URegionClientSubsystem>())
	{
		RegionSubsystem->OnMaterialsBatchReceived(Materials);
	}
#endif
}
