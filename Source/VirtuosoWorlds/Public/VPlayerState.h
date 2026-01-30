#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "VPlayerState.generated.h"

UCLASS()
class AVPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AVPlayerState();

	UPROPERTY(Replicated)
	FString UserID;

	UPROPERTY(Replicated)
	float MaxSpatialDistance = 0.f;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AVPlayerState, UserID);
		DOREPLIFETIME(AVPlayerState, MaxSpatialDistance);
	}
};
