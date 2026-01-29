#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

// IMPORTANT:
// These types are used in interface method signatures and must be fully defined
// in any translation unit that includes this header (e.g., UHT-generated code).
// Use module-relative includes so this header is robust across module include paths.
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"
#include "Model/Package/VMCollision.h"
#include "VRegionServerBridgeEndpoint.generated.h"

/**
 * Interface implemented by the owning server-side controller to receive region streaming results.
 *
 * Hosted in VWShared to avoid coupling VWServer -> VirtuosoWorlds.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UVRegionServerBridgeEndpoint : public UInterface
{
	GENERATED_BODY()
};

class VWSERVER_API IVRegionServerBridgeEndpoint
{
	GENERATED_BODY()

public:
	virtual void ServerSendSpatialItems(const TArray<FVMSpatialItemNet>& Items, bool bHasMore) = 0;
	virtual void ServerSendMaterialsBatch(const TArray<FVMMaterial>& Materials) = 0;
	virtual void ServerSendCollisionsBatch(const TArray<FVMCollision>& Collisions) = 0;
};
