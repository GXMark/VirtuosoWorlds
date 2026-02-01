#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Package/VMMaterial.h"
#include "VRegionServerBridge.generated.h"


/**
 * Server-side bridge object that centralizes region streaming logic and database access.
 *
 * Header lives in VWShared so server code can be referenced by transport classes (e.g. PlayerController)
 * without creating module cycles. Implementation lives in VWServer.
 */
UCLASS()
class VWSERVER_API URegionServerBridge : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UWorld* InWorld);

	// Materials
	void HandleMaterialsRequest(
		APlayerController* PC,
		const TArray<FGuid>& MaterialIds) const;

private:
	UPROPERTY()
	TObjectPtr<UWorld> World;
};
