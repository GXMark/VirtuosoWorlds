#pragma once

#include "CoreMinimal.h"
#include "MPAssetRef.generated.h"

USTRUCT()
struct VW_CREATORS_PLUGIN_API FMPAssetRef
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
	
	FMPAssetRef() {}

	FMPAssetRef(const FGuid& InID, const FName& InName, const FName& InType, const FName& InPath, const FName& InMeta):
		id(InID)
		, name(InName)
		, type(InType)
		, path(InPath)
		, meta(InMeta)
	{}
};
