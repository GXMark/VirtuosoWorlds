#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"

class VWCLIENT_API FVCrunchUtility
{
public:
	static void LoadCRNLibrary();
	static void FreeCRNLibrary();
	static bool IsCRNLibraryLoaded();
	static bool LoadCRNSync(const FGuid& InID, const FName& InName, const bool& InLinear, UTexture2D*& OutTexture);

	// Crunch Methods
	static TArray<uint8> CrunchTexture(UTexture2D* InTexture, bool InSRGB, uint32 InQuality, bool InCreateMipMaps, bool InVerbose);
	static TArray<uint8> CRN_To_DDS(TArray<uint8> InCRNArray, bool InVerbose);
	static UTexture2D* LoadTextureDDS(TArray<uint8> InDDSArray, bool InSRGB, bool InUseMipMaps, bool InVerbose);
	static FString PixelFormatToString(EPixelFormat InPixelFormat);
	static EPixelFormat StringToPixelFormat(const FString& InPixelFormat);
	
private:

	// CRN LIB (Crunch) method binding definitions

	typedef intptr_t* (*crn_compress_ptr)(uintptr_t* image, uint32 width, uint32, uint32 format, uint32 flags, uint32 quality, uint32 threads, uint32 create_mipmaps, uint32& compressed_size, uint32& quality_level, float& bitrate);
	static crn_compress_ptr crn_compress;

	typedef void (*crn_free_block_ptr)(intptr_t* ptr);
	static crn_free_block_ptr crn_free_block;

	typedef intptr_t* (*crn_decompress_ptr)(intptr_t* ptr, uint32& file_size);
	static crn_decompress_ptr crn_decompress;

	static void* DLLHandle;

	static bool GetTextureData(UTexture2D* InTexture, TArray<uint8>& OutArray);
	static bool SetTextureData(UTexture2D* InTexture, const TArray<uint32>& InDXTData, EPixelFormat InFormat, bool InUseMipMaps);
};