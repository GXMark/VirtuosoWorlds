#pragma once

#include "CoreMinimal.h"

struct FVAssetRequest
{
	FGuid ID;
	FName Name;
	FName Type;
	FName Path;
	
	FVAssetRequest() : 
		ID(FGuid(0,0,0,0))
		, Name(NAME_None)
		, Type(NAME_None)
		, Path(NAME_None)
	{}
	
	FVAssetRequest(const FGuid& InID, const FName& InName, const FName& InType, const FName& InPath) : 
		ID(InID)
		, Name(InName)
		, Type(InType)
		, Path(InPath)
	{}
	
	FString GetAssetKey() const
	{
		if (Type == FAssetType::Package)
			return FString::Printf(TEXT("%s.package"),*ID.ToString());
		
		if (Type == FAssetType::Mesh)
			return FString::Printf(TEXT("%s.mesh"),*ID.ToString());

		if (Type == FAssetType::Texture)
			return FString::Printf(TEXT("%s.texture"),*ID.ToString());

		return "";
	}
};