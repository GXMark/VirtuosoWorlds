#pragma once
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

class VWCLIENT_API FVPAKUtility
{
public:
	static bool Register();	// Not used
	static bool Mount(const FString& InPAKPath, const FString& InAssetID);

	// Load asset synchronously
	template <typename T>
	static T* LoadAssetSync_Typed(const FSoftObjectPath& InAssetPath)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value,
		              "T must derive from UObject");

		TSoftObjectPtr<T> AssetPtr(InAssetPath);

		return AssetPtr.LoadSynchronous();
	}

	// Load asset asynchronously
	template <typename T>
	static void LoadAssetAsync(
		const FSoftObjectPath& InAssetPath,
		TFunction<void(T* Loaded)> OnLoaded
	)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value,
		              "T must derive from UObject");

		FStreamableManager& Streamable =
			UAssetManager::Get().GetStreamableManager();

		TSoftObjectPtr<T> AssetPtr(InAssetPath);

		Streamable.RequestAsyncLoad(
			AssetPtr.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([AssetPtr, OnLoaded]()
			{
				T* Loaded = AssetPtr.Get();
				OnLoaded(Loaded);
			})
		);
	}


	// Load PAK asset synchronously
	template <typename T>
	static T* LoadPAKSync(const FGuid& InID, const FName& InName)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value,
			  "T must derive from UObject");
		
		FSoftObjectPath SoftObjectPath;

		SoftObjectPath.SetPath(FString::Printf(TEXT("/Game/%s/%s.%s"), *InID.ToString(), *InName.ToString(), *InName.ToString()));

		if (SoftObjectPath.IsNull())
		{
			return nullptr;
		}

		return Cast<T>(SoftObjectPath.TryLoad());
	}

	// Load PAK asset asynchronously
	template <typename T>
	static void LoadPAKAsync(const FString& InAssetKey, TFunction<void(T*)> OnLoaded)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value,
			  "T must derive from UObject");
		
		FSoftObjectPath SoftObjectPath = FSoftObjectPath(InAssetKey);

		if (SoftObjectPath.IsNull())
			OnLoaded(nullptr);

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		// Request the asset to be loaded asynchronously
		Streamable.RequestAsyncLoad(SoftObjectPath,
		                            FStreamableDelegate::CreateLambda([SoftObjectPath,OnLoaded]()
		                            {
			                            // Callback when the asset is loaded
			                            UObject* LoadedAsset = SoftObjectPath.ResolveObject();

			                            if (T* TypedAsset = Cast<T>(LoadedAsset))
			                            {
				                            OnLoaded(TypedAsset);
			                            }
			                            else
			                            {
				                            UE_LOG(LogTemp, Error, TEXT("Asset Service - Load PAK Async [ Failed to load or cast asset at path: %s ]"), *SoftObjectPath.ToString());
				                            OnLoaded(nullptr);
			                            }
		                            }),
		                            FStreamableManager::AsyncLoadHighPriority
		);
	}
};
