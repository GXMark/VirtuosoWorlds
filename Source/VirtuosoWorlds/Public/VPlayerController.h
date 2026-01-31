#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Types used in UFUNCTION / interface signatures must be fully defined before *.generated.h
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"

#if WITH_CLIENT_CODE
#include "Region/VRegionClient.h"
#include "Region/VRegionClientBridgeEndpoint.h"
class URegionClientBridge; // forward declare
#endif

#if WITH_SERVER_CODE
#include "Region/VRegionServerBridgeEndpoint.h"
class URegionServerBridge; // forward declare
#endif

#include "VPlayerController.generated.h"

class URegionServerBridge;
class UInputMappingContext;

/**
 *  Basic PlayerController class for a third-person game
 *  Manages input mappings
 */
UCLASS(Blueprintable)
class VIRTUOSOWORLDS_API AVPlayerController : public APlayerController
#if WITH_CLIENT_CODE
    , public IVRegionClientBridgeEndpoint
#endif
#if WITH_SERVER_CODE
    , public IVRegionServerBridgeEndpoint
#endif
{
	GENERATED_BODY()
	
public:
	AVPlayerController();

#if WITH_CLIENT_CODE
    // IVRegionClientBridgeEndpoint
    virtual void ClientRequestSpatialItems(const FVector& Origin, int32 MaxItems) override;
    virtual void ClientRequestMaterialsBatch(const TArray<FGuid>& MaterialIds) override;
#endif

#if WITH_SERVER_CODE
	// IVRegionServerBridgeEndpoint
	virtual void ServerSendSpatialItems(const TArray<FVMSpatialItemNet>& Items, bool bHasMore) override;
	virtual void ServerSendMaterialsBatch(const TArray<FVMMaterial>& Materials) override;
#endif

	// RPC Contract
	
	// Client → Server
	UFUNCTION(Server, Reliable)
	void ServerRegionState();

	// Client → Server
	UFUNCTION(Server, Reliable)
	void ServerSpawnPlayer();


	// Spatial streaming (server-authoritative)
	UFUNCTION(Server, Reliable)
	void ServerRequestSpatialItems(const FVector& Origin, int32 MaxItems);

	UFUNCTION(Client, Reliable)
	void ClientReceiveSpatialItems(const TArray<FVMSpatialItemNet>& Items, bool bHasMore);

	// Materials (batched)
	UFUNCTION(Server, Reliable)
	void ServerRequestMaterialsBatch(const TArray<FGuid>& MaterialIds);

	UFUNCTION(Client, Reliable)
	void ClientReceiveMaterialsBatch(const TArray<FVMMaterial>& Materials);

	UFUNCTION(BlueprintCallable, Category="Spatial")
	float GetSpatialDistancePercent() const;

	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* IMC_Default;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* IMC_MouseLook;
	
	virtual void OnRep_PlayerState() override;
	
	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	void OnLeftMouseDown();
	void OnLeftMouseUp();
	
	virtual void BeginPlay() override;
	virtual void InitPlayerState() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Client
	UFUNCTION()
	void OnSpawnPlayer() const;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> GameWidgetClass;
	static AVPlayerController* Get(const UObject* WorldContext);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	}
private:
	
	UPROPERTY()
	class UInputAction* LeftClickAction;

	// Optional: server-side radius filter for spatial streaming (0 disables).
	float SpatialStreamRadiusCm = 0.f;

	UPROPERTY()
	TObjectPtr<URegionServerBridge> RegionServerBridge;

	UPROPERTY()
	TObjectPtr<AVRegionClient> RegionClient;

	float MaxSpatialDistance = 0.f;
	int32 SpatialItemsReceived = 0;
	bool bSpawnedAtSpatialThreshold = false;

};
