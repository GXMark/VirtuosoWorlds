#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

// IMPORTANT:
// These types are used in interface method signatures and must be fully defined
// in any translation unit that includes this header (e.g., UHT-generated code).
/*#include "VWShared/Public/Model/Network/VMSpatialItemNet.h"
#include "VWShared/Public/Model/Package/VMMaterial.h"*/

#include "VRegionClientBridgeEndpoint.generated.h"

/**
 * Interface implemented by the owning client controller to issue region streaming requests.
 *
 * Hosted in VWClient to avoid a module dependency from VWClient back to VirtuosoWorlds.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UVRegionClientBridgeEndpoint : public UInterface
{
	GENERATED_BODY()
};

class VWCLIENT_API IVRegionClientBridgeEndpoint
{
	GENERATED_BODY()

public:
	/** Request a batch of material items by id. */
	virtual void ClientRequestMaterialsBatch(const TArray<FGuid>& MaterialIds) = 0;

};
