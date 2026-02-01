// VWClient/Public/Region/VRegionBridge.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VRegionClientBridge.generated.h"

/**
 * Region bridge used by the region client subsystem to issue material requests through the owning controller.
 *
 * This keeps VWClient independent of the VirtuosoWorlds module.
 */
UCLASS()
class VWCLIENT_API URegionClientBridge : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* InOwningController);

	bool IsValidBridge() const;

	void RequestMaterialsBatch(const TArray<FGuid>& MaterialIds) const;

private:
	UPROPERTY()
	TObjectPtr<APlayerController> OwningController;
};
