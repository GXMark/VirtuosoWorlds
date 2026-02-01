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
