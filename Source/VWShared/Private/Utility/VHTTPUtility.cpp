#include "Utility/VHTTPUtility.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/SecureHash.h"
#include "Misc/FileHelper.h"

// HTTP URL download object synchronously
bool FVHTTPUtility::DownloadObjectSync(
	const FString& InCachePath,
	const FString& InBaseURL,
	const FString& InPassword,
	const FString& InObjectKey)
{
	const FString URL = FString::Printf(TEXT("%s/%s"), *InBaseURL, *InObjectKey);

	// Create the HTTP request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("X-Custom-Auth-Key"), InPassword);

	// Blocks the current thread until the request completes
	HttpRequest->ProcessRequestUntilComplete();

	FHttpResponsePtr HTTPResponse = HttpRequest->GetResponse();

	if (HTTPResponse.IsValid() && HTTPResponse->GetResponseCode() == 200)
	{
		TArray<uint8> RawData = HTTPResponse->GetContent();
		FString AssetPath = FString::Printf(TEXT("%s/%s"), *InCachePath, *InObjectKey);

		if (FFileHelper::SaveArrayToFile(RawData, *AssetPath))
		{
			UE_LOG(LogTemp, Log, TEXT("Asset Loader - HTTP Download Synchronous [ URL: %s, Saved Asset: %s ]"), *URL, *AssetPath);
			return true;
		}

		UE_LOG(LogTemp, Error, TEXT("Asset Loader - HTTP Download Synchronous [ Not Saved, URL: %s, Saved Asset: %s ]"), *URL, *AssetPath);

		return false;
	}

	FString ResponseMessage = HTTPResponse.IsValid() ? FString::FromInt(HTTPResponse->GetResponseCode()) : TEXT("No response");
	UE_LOG(LogTemp, Error, TEXT("Asset Loader - HTTP Download Synchronous [ Download Error, Response Code: %s, Object Key: %s ]"), *ResponseMessage, *InObjectKey);
	return false;
}

// HTTP URL download object asynchronously
void FVHTTPUtility::DownloadObjectAsync(
	const FString& InCachePath,
	const FString& InBaseURL,
	const FString& InPassword,
	const FString& InObjectKey,
	TFunction<void(bool)> Callback)
{
	const FString URL = FString::Printf(TEXT("%s/%s"), *InBaseURL, *InObjectKey);

	// Send the request

	UE_LOG(LogTemp, Log, TEXT("HTTP Utility - Get Object Async [ URL: %s ]"), *URL);

	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("X-Custom-Auth-Key"), InPassword);
	//HttpRequest->SetHeader(TEXT("Accept"), TEXT("application/octet-stream"));

	HttpRequest->OnProcessRequestComplete().BindLambda([Callback, InCachePath, InObjectKey](FHttpRequestPtr Request, const FHttpResponsePtr& HTTPResponse, bool bWasSuccessful)
	{
		// Game Thread

		const bool bHasResponse = HTTPResponse.IsValid();
		const int32 ResponseCode = bHasResponse ? HTTPResponse->GetResponseCode() : 0;

		const FString ResponseMessage = bHasResponse ? HTTPResponseMessage(ResponseCode) : TEXT("No response");

		// Make sure "valid" implies the response exists.

		if (bWasSuccessful && bHasResponse && EHttpResponseCodes::IsOk(ResponseCode))
		{
			TArray<uint8> RawData = HTTPResponse->GetContent();

			FString AssetPath = FString::Printf(TEXT("%s/%s"), *InCachePath, *InObjectKey);

			// Save the file on a background thread
			Async(EAsyncExecution::Thread, [RawData = MoveTemp(RawData), AssetPath, Callback, InObjectKey]
			{
				if (FFileHelper::SaveArrayToFile(RawData, *AssetPath))
				{
					UE_LOG(LogTemp, Log, TEXT("HTTP Utility - Get Object Async [ Saved Asset: %s ]"), *AssetPath);
					Async(EAsyncExecution::TaskGraphMainThread, [Callback] { Callback(true); });
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("HTTP Utility - Get Object Async [ Asset not saved: %s ]"), *AssetPath);
					Async(EAsyncExecution::TaskGraphMainThread, [Callback] { Callback(false); });
				}
			});
		}
		else
		{
			// Ensure the caller always completes even on failure (and never deref HTTPResponse)
			UE_LOG(LogTemp, Warning,
			       TEXT("HTTP Utility - Get Object Async failed. Success=%d HasResponse=%d Code=%d Msg=%s"),
			       bWasSuccessful ? 1 : 0, bHasResponse ? 1 : 0, ResponseCode, *ResponseMessage);
			
			Async(EAsyncExecution::TaskGraphMainThread, [Callback] { Callback(false); });
		}
	});

	HttpRequest->ProcessRequest();
}

// HTTP response message
FString FVHTTPUtility::HTTPResponseMessage(int32 InResponseCode)
{
	switch (InResponseCode)
	{
	case 200:
		return TEXT("200 - OK");
	case 404:
		return TEXT("404 - Not Found");
	case 403:
		return TEXT("403 - Forbidden");
	case 401:
		return TEXT("401 - Unauthorized");
	default:
		return FString::Printf(TEXT("%d - Unknown"), InResponseCode);
	}
}
