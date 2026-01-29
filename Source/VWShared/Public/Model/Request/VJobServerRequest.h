#pragma once

#include "CoreMinimal.h"
#include "Model/Package/VMActor.h"
#include "Model/Package/VMMaterial.h"
#include "VJobServerRequest.generated.h"

USTRUCT()
struct FVJobServerRequest
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FVMActor> ActorItems;
	
	UPROPERTY()
	TArray<FVMMaterial> MaterialItems;

	FVJobServerRequest()
	{}
	
	FVJobServerRequest(const TArray<FVMActor>& InActorItems, const TArray<FVMMaterial>& InMaterialItems) :
		ActorItems(InActorItems)
		, MaterialItems(InMaterialItems)
	{}
};
