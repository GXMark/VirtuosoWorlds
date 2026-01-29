#pragma once
#include "CoreMinimal.h"

class VWSHARED_API FVHTTPUtility
{
public:

	static bool DownloadObjectSync(const FString& InCachePath, const FString& InBaseURL, const FString& InPassword, const FString& InObjectKey);
	static void DownloadObjectAsync(const FString& InCachePath, const FString& InBaseURL, const FString& InPassword, const FString& InObjectKey, TFunction<void(bool)> Callback);
	static FString HTTPResponseMessage(int32 InResponseCode);
};
