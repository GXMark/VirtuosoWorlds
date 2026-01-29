#pragma once

#include "CoreMinimal.h"
#include "VMAssetRef.generated.h"

USTRUCT()
struct VWSHARED_API FVMAssetRef
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid id;
	
	UPROPERTY()
	FName name;
	
	UPROPERTY()
	FName type;
		
	UPROPERTY()
	FName path;
	
	UPROPERTY()
	FName meta;
	
	FVMAssetRef() {}

	FVMAssetRef(const FGuid& InID, const FName& InName, const FName& InType, const FName& InPath, const FName& InMeta):
		id(InID)
		, name(InName)
		, type(InType)
		, path(InPath)
		, meta(InMeta)
	{}
};