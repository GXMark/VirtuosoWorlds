 #pragma once

#include "CoreMinimal.h"
#include "Model/Land/VLocation.h"
#include "VLoginInfo.generated.h"

 USTRUCT()
struct VWSHARED_API FVLoginInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;	// player user name (unique)

	UPROPERTY()
	FVLocation location;	// location

	FVLoginInfo() {}

	FVLoginInfo(const FString& UserID, const FVLocation& Location): user_id(UserID), location(Location)
	{
	}
};
