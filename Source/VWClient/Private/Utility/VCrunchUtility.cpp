#include "Utility/VCrunchUtility.h"
#include "TextureResource.h"
#include "Engine/Texture2D.h"
#include "Misc/Paths.h"
#include "Model/Constant/VConstants.h"
#include "Subsystem/VAssetManager.h"
#include "Utility/VFileUtility.h"

// Static Definitions

typedef intptr_t* (*crn_compress_ptr)(uintptr_t* image, uint32 width, uint32, uint32 format, uint32 flags, uint32 quality, uint32 threads, uint32 create_mipmaps, uint32& compressed_size, uint32& quality_level, float& bitrate);
crn_compress_ptr FVCrunchUtility::crn_compress;

typedef void (*crn_free_block_ptr)(intptr_t* ptr);
crn_free_block_ptr FVCrunchUtility::crn_free_block;

typedef intptr_t* (*crn_decompress_ptr)(intptr_t* ptr, uint32& file_size);
crn_decompress_ptr FVCrunchUtility::crn_decompress;

void* FVCrunchUtility::DLLHandle;

/**
 * Load the CRN Library DLL at runtime
 * Note: IsCRNLibraryLoaded checks if the DLL was loaded 
 */
void FVCrunchUtility::LoadCRNLibrary()
{
	FString DLLPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/crnlib.dll"));

	DLLHandle = FPlatformProcess::GetDllHandle(*DLLPath);

	if (DLLHandle)
	{
		crn_compress = static_cast<crn_compress_ptr>(FPlatformProcess::GetDllExport(DLLHandle, TEXT("crn_compress")));
		crn_free_block = static_cast<crn_free_block_ptr>(FPlatformProcess::GetDllExport(DLLHandle, TEXT("crn_free_block")));
		crn_decompress = static_cast<crn_decompress_ptr>(FPlatformProcess::GetDllExport(DLLHandle, TEXT("crn_decompress")));
	}
}

/**
 * Unloads an already loaded CRN DLL
 */
void FVCrunchUtility::FreeCRNLibrary()
{
	if (DLLHandle)
	{
		FPlatformProcess::FreeDllHandle(DLLHandle);
		DLLHandle = nullptr;
	}
}

/**
 * Check the CRN DLL loaded at runtime
 * @return DLL Loaded = true, DLL Not Loaded = false
 */
bool FVCrunchUtility::IsCRNLibraryLoaded()
{
	return DLLHandle ? true : false;
}


/**
 * Crunch a texture of pixel format (PF_DXT1, PF_DXT5, PF_BC5, PF_B8G8R8A8)
 * @param InTexture - Source texture to use for the crunch compression
 * @param InSRGB - Texture space ( eg. Albedo, Metallic, Specular SRGB=true,  Normal map, AO, Packed Channels  SRGB=false )
 * @param InQuality - Compress quality range ( 0 - Low, 128 - Medium, 255 - High )
 * @param InCreateMipMaps - Create mip map flag
 * @param InVerbose - Show output log information 
 * @return Crunch byte array
 */
TArray<uint8> FVCrunchUtility::CrunchTexture(UTexture2D* InTexture, bool InSRGB, uint32 InQuality, bool InCreateMipMaps, bool InVerbose)
{
	TArray<uint8> SrcTextureArray;

	EPixelFormat SrcPixelFormat = InTexture->GetPixelFormat();

	// Check supported texture formats (PF_DXT1, PF_DXT5, PF_BC5, and PF_B8G8R8A8 (uncompressed))
	if (SrcPixelFormat != PF_DXT1 &&
		SrcPixelFormat != PF_DXT5 &&
		SrcPixelFormat != PF_BC5 &&
		SrcPixelFormat != PF_B8G8R8A8)
	{
		UE_LOG(LogTemp, Error, TEXT("Crunch Texture - [ Status: Invalid Crunch Texture Format : %s ]"), *PixelFormatToString(InTexture->GetPixelFormat()));
		return TArray<uint8>();
	}

	// CRN Formats
	// 0 - DXT1
	// 2 - DXT5
	// 7 - BC5/ATI2

	// SRGB Codes
	// 10 - Non Linear
	// 11 - Linear

	uint32 Width = InTexture->GetPlatformData()->SizeX;
	uint32 Height = InTexture->GetPlatformData()->SizeY;
	uint32 Format = (SrcPixelFormat == PF_DXT1 || SrcPixelFormat == PF_B8G8R8A8) ? 0 : SrcPixelFormat == PF_DXT5 ? 2 : 7;

	if (GetTextureData(InTexture, SrcTextureArray))
	{
		// Allocate the unmanaged array
		void* UnmanagedMemory = FMemory::Malloc(SrcTextureArray.Num());

		// Initialize memory
		FMemory::Memset(UnmanagedMemory, 0, SrcTextureArray.Num());

		// Copy the byte array over to the unmanaged array
		FMemory::Memcpy(UnmanagedMemory, SrcTextureArray.GetData(), SrcTextureArray.Num());

		uint32 CompressionSize;
		uint32 QualityLevel;
		uint32 Threads = 16;
		float Bitrate;
		intptr_t* CRNDataPtr = crn_compress(
			static_cast<uintptr_t*>(UnmanagedMemory),
			Width,
			Height,
			Format,
			InSRGB ? 10 : 11,
			InQuality,
			Threads,
			InCreateMipMaps ? 1 : 0,
			CompressionSize,
			QualityLevel,
			Bitrate);

		// Allocate space for the crunched byte array
		TArray<uint8> CRNArray;
		CRNArray.SetNum(CompressionSize);

		// Copy the result into the result
		FMemory::Memcpy(CRNArray.GetData(), CRNDataPtr, CRNArray.Num());

		// Free the memory block
		crn_free_block(CRNDataPtr);

		// Free the memory
		FMemory::Free(UnmanagedMemory);

		if (InVerbose)
		{
			UE_LOG(LogTemp, Log, TEXT("Crunch Texture - [ Format: %s  Width: %d  Height: %d  SRGB: %s  Source Size: %d (kb)  CRN Size: %d (kb)  Threads: %d  Quality: %d  Bit Rate: %f ]"),
			       *PixelFormatToString(SrcPixelFormat),
			       Width,
			       Height,
			       InSRGB ? TEXT("true") : TEXT("false"),
			       SrcTextureArray.Num()/1024,
			       CompressionSize/1024,
			       Threads,
			       InQuality,
			       Bitrate);
		}

		return CRNArray;
	}

	return TArray<uint8>();
}

/**
 * Converts the CRN byte array into a DDS (DXT) byte array
 * @param InCRNArray - Crunch byte array
 * @param InVerbose - Show logs
 * @return DDS byte array
 */
TArray<uint8> FVCrunchUtility::CRN_To_DDS(TArray<uint8> InCRNArray, bool InVerbose)
{
	// Allocate the unmanaged array
	void* UnmanagedMemory = FMemory::Malloc(InCRNArray.Num());

	// Copy the byte array over to the unmanaged array
	FMemory::Memcpy(UnmanagedMemory, InCRNArray.GetData(), InCRNArray.Num());

	uint32 DDSFileSize = InCRNArray.Num();

	// Get the DDS data from the CRN data
	intptr_t* DDSDataPtr = crn_decompress(static_cast<intptr_t*>(UnmanagedMemory), DDSFileSize);

	// Allocate space for the dds byte array
	TArray<uint8> DDSArray;
	DDSArray.SetNum(DDSFileSize);

	// Copy the result into the result
	FMemory::Memcpy(DDSArray.GetData(), DDSDataPtr, DDSArray.Num());

	// Free the memory block
	crn_free_block(DDSDataPtr);

	// Free the memory
	FMemory::Free(UnmanagedMemory);

	if (InVerbose)
		UE_LOG(LogTemp, Log, TEXT("CRN to DDS - [ CRN Size: %d (kb) to DDS Size : %d (kb) ]"), InCRNArray.Num()/1024, DDSArray.Num()/1024);

	return DDSArray;
}

/**
 * Load the texture from the DDS (DXT) byte array
 * @param InDDSArray - DDS byte array
 * @param InSRGB - Colorspace of the created texture
 * @param InUseMipMaps - Add the mip maps to the created texture when they exist
 * @param InVerbose - Show logs
 * @return The created DXT texture
 */
UTexture2D* FVCrunchUtility::LoadTextureDDS(TArray<uint8> InDDSArray, bool InSRGB, bool InUseMipMaps, bool InVerbose)
{
	// Discover the format from the DDS data
	const FString formatStr = FString::Printf(TEXT("%c%c%c%c"),
	                                          static_cast<TCHAR>(InDDSArray[84]),
	                                          static_cast<TCHAR>(InDDSArray[85]),
	                                          static_cast<TCHAR>(InDDSArray[86]),
	                                          static_cast<TCHAR>(InDDSArray[87]));

	// Supported formats from DDS are (DXT1, DXT5 or ATI2 (BC5))
	if (formatStr == "DXT1" || formatStr == "DXT5" || formatStr == "ATI2")
	{
		// This header byte should be 124 for DDS image files
		uint8 ddsSizeCheck = InDDSArray[4];
		if (ddsSizeCheck == 124)
		{
			int32 DDSWidth = InDDSArray[17] * 256 + InDDSArray[16];
			int32 DDSHeight = InDDSArray[13] * 256 + InDDSArray[12];

			int32 DDS_HEADER_SIZE = 128;
			TArray<uint32> DXTArray;
			DXTArray.SetNum(InDDSArray.Num() - DDS_HEADER_SIZE);

			size_t sourceOffset = DDS_HEADER_SIZE;
			// Copy the DDS Array without its header (this is the DXT Byte Array)
			FMemory::Memcpy(DXTArray.GetData(), InDDSArray.GetData() + sourceOffset, DXTArray.Num());

			EPixelFormat PixelFormat;
			if (formatStr == "DXT1")
				PixelFormat = PF_DXT1;
			else if (formatStr == "DXT5")
				PixelFormat = PF_DXT5;
			else
				PixelFormat = PF_BC5;

			UTexture2D* Texture = UTexture2D::CreateTransient(DDSWidth, DDSHeight, PixelFormat);
			Texture->SRGB = InSRGB;

			if (SetTextureData(Texture, DXTArray, PixelFormat, InUseMipMaps))
			{
				if (InVerbose)
				{
					UE_LOG(LogTemp, Log, TEXT("Load Texture DDS - [ Format: %s  Width: %d  Height: %d  Use Mip Maps: %s ]"),
					       *formatStr, DDSWidth, DDSHeight, InUseMipMaps ? TEXT("true" : TEXT("false")));
				}
			}

			return Texture;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Load Texture DDS - [ Status: Invalid DDS Format : %s  Supports (DXT1, DXT5, ATI2 (BC5)) ]"), *formatStr);
	}

	return nullptr;
}


// Private

/**
 * Get the dxt pixel data from the texture and remaps the channels into an uint8 array.
 * @param InTexture - Compressed texture (DXT1, DXT5, BC5, B8G8R8A8)
 * @param OutArray - Texture byte array
 * @return true - success, false - fail
 */
bool FVCrunchUtility::GetTextureData(UTexture2D* InTexture, TArray<uint8>& OutArray)
{
	if (InTexture->GetPixelFormat() != PF_DXT1 &&
		InTexture->GetPixelFormat() != PF_DXT5 &&
		InTexture->GetPixelFormat() != PF_BC5 &&
		InTexture->GetPixelFormat() != PF_B8G8R8A8)
	{
		UE_LOG(LogTemp, Error, TEXT("Get Texture Data - [ Status: Invalid texture format (requires DXT1,DXT5,BC5). %s ]"), *PixelFormatToString(InTexture->GetPixelFormat()));
		return false;
	}

	// Update texture with no mip maps and decompress (if required)
	TextureCompressionSettings OldCompressionSettings = InTexture->CompressionSettings;

#if WITH_EDITOR
	TextureMipGenSettings OldMipGenSettings = InTexture->MipGenSettings;
	InTexture->MipGenSettings = TMGS_NoMipmaps;
#else
	ETextureMipLoadOptions OldMipLoadOptions = InTexture->MipLoadOptions;
	InTexture->MipLoadOptions = ETextureMipLoadOptions::OnlyFirstMip;
#endif

	InTexture->CompressionSettings = TC_VectorDisplacementmap;

	InTexture->UpdateResource();

	FlushRenderingCommands(); // Ensure that the update resource has been updated

	//FPlatformProcess::Sleep(1); // Required to ensure the platform data is available

	FTexturePlatformData* PlatformData = InTexture->GetPlatformData();

	// Get the texture data
	FTexture2DMipMap& MipMap = PlatformData->Mips[0];

	// Protect the texture data for reading
	if (void* Data = MipMap.BulkData.Lock(LOCK_READ_ONLY))
	{
		int32 Width = PlatformData->SizeX;
		int32 Height = PlatformData->SizeY;

		// Allocate space for the texture data
		OutArray.SetNum(Width * Height * 4);

		// Get a pixel data byte pointer to the image
		uint8* PixelData = static_cast<uint8*>(Data);

		// Add the pixel data
		for (int32 y = 0; y < Height; y++)
		{
			for (int32 x = 0; x < Width; x++)
			{
				int Index = (y * Width + x) * 4;
				OutArray[Index] = PixelData[Index + 2]; // R
				OutArray[Index + 1] = PixelData[Index + 1]; // G
				OutArray[Index + 2] = PixelData[Index]; // B
				OutArray[Index + 3] = PixelData[Index + 3]; // A
			}
		}

		//UE_LOG(LogTemp, Error, TEXT("Get Texture Data - [ Status: Success, Name: %s, Width: %d,  Height:%d ]"), *Texture->GetName(), Width, Height);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Get Texture Data - [ Status: Invalid data ]"));
	}

	MipMap.BulkData.Unlock();

	// Restore the original texture mip gen settings
	InTexture->CompressionSettings = OldCompressionSettings;

#if WITH_EDITOR
	InTexture->MipGenSettings = OldMipGenSettings;
#else
	InTexture->MipLoadOptions = OldMipLoadOptions;
#endif

	InTexture->UpdateResource();

	return true;
}

/**
 * Set the dxt byte array into the dxt formatted texture
 * @param InTexture - Target texture
 * @param InDXTData - DXT data (which may include its mip data if generated during crunch)
 * @param InFormat - Supported Formats are PF_DXT1, PF_DXT5 and PF_BC5 (Normal Map)
 * @param InUseMipMaps - Add the mip maps if they exist in the dxt data
 * @return true - success, false - fail
 */
bool FVCrunchUtility::SetTextureData(UTexture2D* InTexture, const TArray<uint32>& InDXTData, EPixelFormat InFormat, bool InUseMipMaps)
{
	if (!InTexture)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid texture."));
		return false;
	}

	FTexturePlatformData* PlatformData = InTexture->GetPlatformData();

	int32 TotalMipBytes = 0;
	int32 Width = PlatformData->SizeX;
	int32 Height = PlatformData->SizeY;
	const int32 BlockSize = InFormat == PF_DXT1 ? 8 : 16; // DXT1 - 8  DXT5/BC5 - 16


	/*if(UseMipMaps)
	{
		UE_LOG(LogTemp, Error, TEXT("Format: %s  Width: %d  Height: %d  DXT Array Size: %d bytes"),
			   *PixelFormatToString(Format), Width, Height, DXTData.Num());

		for (int MipIndex = 0; MipIndex < 11; MipIndex++)
		{
			int32 MipWidth = FMath::Max(1, Width >> MipIndex);
			int32 MipHeight = FMath::Max(1, Height >> MipIndex);

			int32 MipSize = ((MipWidth + 3) / 4) * ((MipHeight + 3) / 4) * BlockSize;
			TotalMipBytes += MipSize;
			UE_LOG(LogTemp, Log, TEXT("Mip Index: %d  Width: %d  Height: %d  Size: %d  Total Mip Size: %d"), MipIndex, MipWidth, MipHeight, MipSize, TotalMipBytes);
		}

		TotalMipBytes = 0;
		Width = PlatformData->SizeX;
		Height = PlatformData->SizeY;
	}*/

	// Default compression settings for both DXT1 (BC1) and DXT5 (BC3 with A)
	InTexture->CompressionSettings = InFormat == PF_DXT1 ? TC_Default : TC_Normalmap;

	InTexture->MipLoadOptions = ETextureMipLoadOptions::Default;

	// Maintain a pointer to the dxt data, so we can advance to the next mip data
	const uint32* CurrentPtr = InDXTData.GetData();

	// Copy the base DXT mip 0 data into the already allocated mip 0 in the texture
	FTexture2DMipMap& Mip0 = InTexture->GetPlatformData()->Mips[0];

	int32 MipWidth = FMath::Max(1, Width >> 0);
	int32 MipHeight = FMath::Max(1, Height >> 0);
	
	const int32 Mip0Size = ((MipWidth + 3) / 4) * ((MipHeight + 3) / 4) * BlockSize;

	TotalMipBytes += Mip0Size;
	void* TextureData = Mip0.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, InDXTData.GetData(), Mip0Size);
	Mip0.BulkData.Unlock();

	if (InUseMipMaps)
	{
		constexpr int MaxMips = 11;
		// Iterate through the other mips until none are found
		for (int32 MipIndex = 1; MipIndex < MaxMips; MipIndex++)
		{
			MipWidth = FMath::Max(1, Width >> MipIndex);
			MipHeight = FMath::Max(1, Height >> MipIndex);
			
			// Calculate the size of the mip
			int32 MipSize = ((MipWidth + 3) / 4) * ((MipHeight + 3) / 4) * BlockSize;

			// Check if we have another mip map to create
			if (TotalMipBytes < InDXTData.Num())
			{
				// Advance the pointer to the next mip level in the DXT array
				CurrentPtr += MipSize;

				// Create the mip
				FTexture2DMipMap* Mip = new FTexture2DMipMap();
				Mip->SizeX = MipWidth;
				Mip->SizeY = MipHeight;
				InTexture->GetPlatformData()->Mips.Add(Mip);

				Mip->BulkData.Lock(LOCK_READ_WRITE);
				void* Mip1Data = Mip->BulkData.Realloc(MipSize);
				FMemory::Memcpy(Mip1Data, CurrentPtr, MipSize);
				Mip->BulkData.Unlock();

				TotalMipBytes += MipSize;
			}
			else
			{
				// Break from the mip creation
				break;
			}
		}
	}

	// Update the texture to load the mip data
	InTexture->UpdateResource();

	return true;
}

/**
 * Convert an enumerated texture format to a string format
 * @param InPixelFormat - Texture format as enumeration
 * @return Texture format as string
 */
FString FVCrunchUtility::PixelFormatToString(EPixelFormat InPixelFormat)
{
	if (InPixelFormat == PF_A32B32G32R32F)
		return "PF_A32B32G32R32F";

	if (InPixelFormat == PF_B8G8R8A8)
		return "PF_B8G8R8A8";

	if (InPixelFormat == PF_G8)
		return "PF_G8";

	if (InPixelFormat == PF_G16)
		return "PF_G16";

	if (InPixelFormat == PF_DXT1)
		return "PF_DXT1";

	if (InPixelFormat == PF_DXT3)
		return "PF_DXT3";

	if (InPixelFormat == PF_DXT5)
		return "PF_DXT5";

	if (InPixelFormat == PF_BC4)
		return "PF_BC4";

	if (InPixelFormat == PF_BC5)
		return "PF_BC5";

	if (InPixelFormat == PF_R32_FLOAT)
		return "PF_R32_FLOAT";

	if (InPixelFormat == PF_R16G16B16A16_UINT)
		return "PF_R16G16B16A16_UINT";

	if (InPixelFormat == PF_R8G8)
		return "PF_R8G8";

	if (InPixelFormat == PF_R8G8B8A8)
		return "PF_R8G8B8A8";

	if (InPixelFormat == PF_A2B10G10R10)
		return "PF_A2B10G10R10";

	return "PF_Unknown";
}

/**
 * Convert a texture string format to an enumeration format
 * @param InPixelFormat - Texture format as string
 * @return Texture format as enumeration
 */
EPixelFormat FVCrunchUtility::StringToPixelFormat(const FString& InPixelFormat)
{
	if (InPixelFormat == "PF_A32B32G32R32F")
		return PF_A32B32G32R32F;

	if (InPixelFormat == "PF_B8G8R8A8")
		return PF_B8G8R8A8;

	if (InPixelFormat == "PF_G8")
		return PF_G8;

	if (InPixelFormat == "PF_G16")
		return PF_G16;

	if (InPixelFormat == "PF_DXT1")
		return PF_DXT1;

	if (InPixelFormat == "PF_DXT3")
		return PF_DXT3;

	if (InPixelFormat == "PF_DXT5")
		return PF_DXT5;

	if (InPixelFormat == "PF_BC4")
		return PF_BC4;

	if (InPixelFormat == "PF_BC5")
		return PF_BC5;

	if (InPixelFormat == "PF_R32_FLOAT")
		return PF_R32_FLOAT;

	if (InPixelFormat == "PF_R16G16B16A16_UINT")
		return PF_R16G16B16A16_UINT;

	if (InPixelFormat == "PF_R8G8")
		return PF_R8G8;

	if (InPixelFormat == "PF_R8G8B8A8")
		return PF_R8G8B8A8;

	if (InPixelFormat == "PF_A2B10G10R10")
		return PF_A2B10G10R10;

	return PF_Unknown;
}

// Load CRN (Texture) Synchronously
bool FVCrunchUtility::LoadCRNSync(const FGuid& InID, const FName& InName, const bool& InLinear, UTexture2D*& OutTexture)
{
	// Load the texture asset data from disk-cache
	TArray<uint8> TextureData;
	FVFileUtility::LoadByteArrayFromFile(UVAssetManager::CachePath, InID, FVAssetType::Texture, TextureData);

	// Converts the CRN data to DDS data
	const TArray<uint8> DDS = CRN_To_DDS(TextureData, false);

	OutTexture = LoadTextureDDS(DDS, !InLinear, true, false);
	if (OutTexture)
	{
		UE_LOG(LogTemp, Log, TEXT("Material Comp - Load Texture [ Loaded CRN Texture, Texture Ref (%s, %s), Linear: %s ]"),
			   *InID.ToString(), *InName.ToString(), InLinear?TEXT("true"):TEXT("false"));
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Material Comp - Load Texture [ Failed To Load CRN Texture, Texture Ref (%s, %s), Linear: %s ]"),
		   *InID.ToString(), *InName.ToString(), InLinear?TEXT("true"):TEXT("false"));

	return false;
}