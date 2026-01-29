#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"

class VW_CREATORS_PLUGIN_API FCPCrunchUtility
{
public:
	void LoadCRNLibrary();
	void FreeCRNLibrary();
	bool IsCRNLibraryLoaded() const;

	// Crunch Methods
	TArray<uint8> CrunchTexture(UTexture2D* InTexture, bool InSRGB, uint32 InQuality, bool InCreateMipMaps, bool InVerbose) const;
	TArray<uint8> CRN_To_DDS(TArray<uint8> InCRNArray, bool InVerbose) const;
	static UTexture2D* LoadTextureDDS(TArray<uint8> InDDSArray, bool InSRGB, bool InUseMipMaps, bool InVerbose);
	static FString PixelFormatToString(EPixelFormat InPixelFormat);
	static EPixelFormat StringToPixelFormat(const FString& InPixelFormat);
private:

	// CRN LIB (Crunch) method binding definitions

	typedef intptr_t* (*crn_compress_ptr)(uintptr_t* image, uint32 width, uint32, uint32 format, uint32 flags, uint32 quality, uint32 threads, uint32 create_mipmaps, uint32& compressed_size, uint32& quality_level, float& bitrate);
	crn_compress_ptr crn_compress = nullptr;

	typedef void (*crn_free_block_ptr)(intptr_t* ptr);
	crn_free_block_ptr crn_free_block = nullptr;

	typedef intptr_t* (*crn_decompress_ptr)(intptr_t* ptr, uint32& file_size);
	crn_decompress_ptr crn_decompress = nullptr;

	void* DLLHandle = nullptr;

	static bool GetTextureData(UTexture2D* InTexture, TArray<uint8>& OutArray);
	static bool SetTextureData(UTexture2D* InTexture, const TArray<uint32>& InDXTData, EPixelFormat InFormat, bool InUseMipMaps);
	static void GenerateMipMaps(UTexture2D* InTexture, int32 InMipCount);
};
