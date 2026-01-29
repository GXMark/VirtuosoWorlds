#pragma once

#include "CoreMinimal.h"
#include "VAssetRequest.h"
#include "Model/Constant/VConstants.h"

struct FVMeshAssetRequest : FVAssetRequest
{
	FVMeshAssetRequest() : 
		FVAssetRequest(FGuid(0,0,0,0), NAME_None, NAME_None, NAME_None)
	{}
	
	FVMeshAssetRequest(const FGuid& InID, const FName& InName, const FName& InPath) : 
		FVAssetRequest(InID, InName, FVAssetType::Mesh, InPath)
	{}
};
