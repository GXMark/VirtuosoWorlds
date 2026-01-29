#pragma once

class FVCommonUtility
{
public:
	static bool CheckGUID(const FString& InRawGuid);
	static FVector3d RemoveNegativeZero(const FVector3d& InValue);
};
