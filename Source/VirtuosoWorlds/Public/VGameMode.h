#pragma once

#include "CoreMinimal.h"
#include "VPlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "Model/Land/VParcel.h"
#include "Model/Login/VLoginInfo.h"
#include "VGameMode.generated.h"

UCLASS(Blueprintable)
class VIRTUOSOWORLDS_API AVGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVGameMode();

	void SpawnPlayerPawn(APlayerController* PC, const FVector& Position, const FRotator& Rotation);
	void ShutDownServer() const;

private:
	UPROPERTY()
	TMap<FUniqueNetIdRepl, FString> PendingUserIDs;

	UPROPERTY()
	float SpawnThresholdDistance;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer,ENetRole InRemoteRole,const FString& Portal,const FString& Options,	const FUniqueNetIdRepl& UniqueId,FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
