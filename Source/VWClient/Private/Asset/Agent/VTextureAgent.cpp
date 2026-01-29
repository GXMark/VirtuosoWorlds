#include "Asset/Agent/VTextureAgent.h"
#include "Model/Constant/VConstants.h"
#include "Subsystem/VAssetManager.h"
#include "Utility/VCrunchUtility.h"


// Initialize texture manager
bool UVTextureAgent::Initialize()
{
#if WITH_CLIENT_CODE
	FVCrunchUtility::LoadCRNLibrary();
	if (FVCrunchUtility::IsCRNLibraryLoaded())
	{
		UE_LOG(LogTemp, Log, TEXT("Texture Manager - Initialize [ Crunch DLL loaded ]"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Texture Manager - Initialize [ Crunch DLL not loaded ]"));
		return false;
	}

	TextureMap.Empty();
#endif

	return true;
}

// Destroy CRN load library
void UVTextureAgent::BeginDestroy()
{
	Super::BeginDestroy();

#if WITH_CLIENT_CODE
	// Free the crunch library when using the client
	if (FVCrunchUtility::IsCRNLibraryLoaded())
		FVCrunchUtility::FreeCRNLibrary();
#endif
}

// Is the texture locally cached?
bool UVTextureAgent::IsLocallyCached(const FGuid& InTextureID, const FName InCacheType) const
{
	if (InCacheType == FVCacheType::Memory)
	{
		if (TextureMap.Contains(InTextureID))
			return true;
	}
	else if (InCacheType == FVCacheType::Disk)
	{
		const FString FilePath = UVAssetManager::CachePath / InTextureID.ToString() + TEXT(".texture");
		return FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	}
	return false;
}

// Set shared texture
void UVTextureAgent::SetTexture(const FGuid& InTextureID, UTexture2D* InTexture)
{
	TextureMap.Add(InTextureID, InTexture);
}

// Get texture
UTexture2D* UVTextureAgent::GetTexture(const FGuid& InKey)
{
	if (TextureMap.Contains(InKey))
		return TextureMap[InKey];
	return nullptr;
}


/**
 * Set texture item on the material instance
 * 
 * @param InTextureItem - Texture item
 * @return true - Success, false - Failed
 */
/**bool UTextureManager::SetTextureItem(const FTextureItem& InTextureItem)
{
	if (!MaterialItemMap.Contains(InTextureItem.MaterialID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Comp - Set Texture Instance - Material Item Not Found"));
		return false;
	}

	if (!MaterialMap.Contains(InTextureItem.MaterialID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Comp - Set Texture Instance - Material Instance Not Found"));
		return false;
	}

	FMMaterial MaterialItem = MaterialItemMap[InTextureItem.MaterialID];
	UMaterialInstanceDynamic* MaterialInstance = GetMaterialInstance(InTextureItem.MaterialID);

	// Iterate through the material instances texture items assigning the texture instance when found
	for (FMTexture TextureItem : MaterialItem.textures)
	{
		// Material texture slot matches the texture ref id
		if (TextureItem.id == InTextureItem.Item.id)
		{
			// If the texture instance is nullptr it means its not loaded yet
			if (TextureMap[InTextureItem.Item.id] == nullptr)
			{
				// Load the texture asset data from disk
				TArray<uint8> TextureData;
				FFileUtility::LoadAssetFromFile(UCacheManager::CachePath, InTextureItem.Item.id, InTextureItem.Item.name, TextureData);

				// Converts the CRN data to DDS data
				const TArray<uint8> DDS = FCrunchUtility::CRN_To_DDS(TextureData, false);

				if (UTexture2D* TextureInstance = FCrunchUtility::LoadTextureDDS(DDS, !TextureItem.linear, true, false))
				{
					TextureMap[InTextureItem.Item.id] = TextureInstance;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Material Comp - Set Texture Instance - Failed To Load CRN Texture"));
					return false;
				}
			}

			// Assign the shared texture instance to the shared material
			MaterialInstance->SetTextureParameterValue(TextureItem.property, TextureMap[InTextureItem.Item.id]);

			UE_LOG(LogTemp, Log, TEXT("Material Comp - Set Texture Instance [ Material ID: %s, Texture ID: %s]"),
			       *MaterialItem.id.ToString(), *InTextureItem.Item.id.ToString());
		}
	}

	return true;
}
*/
