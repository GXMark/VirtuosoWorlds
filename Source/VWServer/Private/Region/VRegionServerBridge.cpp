#include "Region/VRegionServerBridge.h"
#include "Region/VRegionServerBridgeEndpoint.h"

#include "VWDatabase/Public/VDataService.h"

#include "Model/Package/VMMaterial.h"

#include "JsonObjectConverter.h"

void URegionServerBridge::Initialize(UWorld* InWorld)
{
	World = InWorld;
}

void URegionServerBridge::HandleMaterialsRequest(
	APlayerController* PC,
	const TArray<FGuid>& MaterialIds) const
{
	if (!World)
	{
		return;
	}

	UVDataService* DataService = UVDataService::Get(World);
	if (!DataService)
	{
		return;
	}

	TArray<FVMMaterial> Results;
	Results.Reserve(MaterialIds.Num());

	for (const FGuid& Id : MaterialIds)
	{
		if (!Id.IsValid())
		{
			continue;
		}

		const FString Json = DataService->GetMaterial(Id.ToString());
		if (Json.IsEmpty())
		{
			continue;
		}

		FVMMaterial Mat;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Mat, 0, 0))
		{
			Results.Add(Mat);
		}
	}

	if (IVRegionServerBridgeEndpoint* Endpoint = Cast<IVRegionServerBridgeEndpoint>(PC))
	{
		Endpoint->ServerSendMaterialsBatch(Results);
	}
}
