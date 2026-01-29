#pragma once

#include "CoreMinimal.h"
#include "VMeshTargetRequest.generated.h"

USTRUCT()
struct FVMeshTargetRequest
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGuid ActorID;
	
	UPROPERTY()
	FGuid AssetID;
		
	FVMeshTargetRequest() : 
		ActorID(FGuid(0,0,0,0))
		, AssetID({})
	{}
	
	FVMeshTargetRequest(const FGuid& InActorID, const FGuid& InAssetID) : 
		ActorID(InActorID)
		, AssetID(InAssetID)
	{}
};