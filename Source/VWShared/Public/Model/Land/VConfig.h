#pragma once

#include "CoreMinimal.h"
#include "VServer.h"
#include "VRegion.h"
#include "VConfig.generated.h"

USTRUCT()
struct VWSHARED_API FVConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FVServer server;

	UPROPERTY()
	FVRegion region;

	FVConfig() {}
	FVConfig(const FVServer& Server, const FVRegion& Region): server(Server), region(Region) {}
};