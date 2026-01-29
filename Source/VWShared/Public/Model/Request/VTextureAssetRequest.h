#pragma once

#include "CoreMinimal.h"
#include "VAssetRequest.h"
#include "Model/Constant/VConstants.h"

struct FVTextureAssetRequest : FVAssetRequest
{
	bool Linear;
	
	FVTextureAssetRequest() : 
		Linear(false)
	{}

	FVTextureAssetRequest(const FGuid& InID, const FName& InName, const FName& InPath, const bool InLinear) : 
		FVAssetRequest(InID, InName, FVAssetType::Texture, InPath)
		, Linear(InLinear)
	{}
};
