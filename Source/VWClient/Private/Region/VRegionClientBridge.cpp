#include "Region/VRegionClientBridge.h"
#include "GameFramework/PlayerController.h"
#include "Region/VRegionClientBridgeEndpoint.h"

void URegionClientBridge::Initialize(APlayerController* InOwningController)
{
	OwningController = InOwningController;
}

bool URegionClientBridge::IsValidBridge() const
{
	return OwningController
		&& OwningController->GetClass()->ImplementsInterface(UVRegionClientBridgeEndpoint::StaticClass());
}

void URegionClientBridge::RequestSpatialItems(const FVector& Origin, int32 MaxItems) const
{
	if (!IsValidBridge())
	{
		return;
	}

	if (IVRegionClientBridgeEndpoint* Endpoint = Cast<IVRegionClientBridgeEndpoint>(OwningController))
	{
		Endpoint->ClientRequestSpatialItems(Origin, MaxItems);
	}
}

void URegionClientBridge::RequestMaterialsBatch(const TArray<FGuid>& MaterialIds) const
{
	if (!IsValidBridge())
	{
		return;
	}

	if (IVRegionClientBridgeEndpoint* Endpoint = Cast<IVRegionClientBridgeEndpoint>(OwningController))
	{
		Endpoint->ClientRequestMaterialsBatch(MaterialIds);
	}
}

void URegionClientBridge::RequestCollisionsBatch(const TArray<FGuid>& CollisionIds) const
{
	if (!IsValidBridge())
	{
		return;
	}

	if (IVRegionClientBridgeEndpoint* Endpoint = Cast<IVRegionClientBridgeEndpoint>(OwningController))
	{
		Endpoint->ClientRequestCollisionsBatch(CollisionIds);
	}
}
