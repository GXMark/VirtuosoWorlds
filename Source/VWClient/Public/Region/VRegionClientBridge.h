// VWClient/Public/Region/VRegionBridge.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VRegionClientBridge.generated.h"

/**
 * Region bridge used by the region client subsystem to issue spatial/material requests through the owning controller.
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

	void RequestSpatialItems(const FVector& Origin, int32 MaxItems) const;
	void RequestMaterialsBatch(const TArray<FGuid>& MaterialIds) const;

private:
	UPROPERTY()
	TObjectPtr<APlayerController> OwningController;
};
