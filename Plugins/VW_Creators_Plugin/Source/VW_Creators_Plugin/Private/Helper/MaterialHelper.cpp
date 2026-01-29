#include "Helper/MaterialHelper.h"
#include "Components/DecalComponent.h"
#include "Constant/Constants.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Misc/FileHelper.h"
#include "Model/Package/MPLinearColor.h"
#include "Helper/CacheHelper.h"
#include "Helper/StatsHelper.h"
#include "Model/Package/MPActor.h"

// Initialize
bool FCPMaterialHelper::ShowLogs = false;
TArray<UMaterialInterface*> FCPMaterialHelper::ParentMaterials = {};
UMaterialInterface* FCPMaterialHelper::DefaultMaterial = nullptr;
TMap<UMaterialInterface*, FMPMaterial> FCPMaterialHelper::SharedMaterialMap = {};
FCPCrunchUtility FCPMaterialHelper::CRNLib = {};

/**
 * Initialize material saver
 * @param ShowOutputLogs - Show logs
 * @return Success - true, Failed - false
 */
bool FCPMaterialHelper::Initialize(bool ShowOutputLogs)
{
	CRNLib.LoadCRNLibrary();
	if (!CRNLib.IsCRNLibraryLoaded())
	{
		UE_LOG(LogTemp, Error, TEXT("Level Loader - CRN Library Not Loaded"));
		return false;
	}

	// Load the materials
	UMaterialInterface* Material1 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/VW_Creators_Plugin/MasterMaterial/Materials/MM_Opaque")));
	UMaterialInterface* Material2 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/VW_Creators_Plugin/MasterMaterial/Materials/MM_Opaque_ARMS")));
	UMaterialInterface* Material3 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/VW_Creators_Plugin/MasterMaterial/Materials/MM_GlassTT")));
	UMaterialInterface* Material4 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/VW_Creators_Plugin/MasterMaterial/Materials/MM_Glass_Ornate")));
	UMaterialInterface* Material5 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/VW_Creators_Plugin/MasterMaterial/Materials/MM_Decal")));
	UMaterialInterface* DefaultMaterial1 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/VW_Creators_Plugin/MasterMaterial/Materials/DefaultMaterial")));

	if (Material1 && Material2 && Material3 && Material4 && Material5 && DefaultMaterial1)
	{
		ParentMaterials.Add(Material1);
		ParentMaterials.Add(Material2);
		ParentMaterials.Add(Material3);
		ParentMaterials.Add(Material4);
		ParentMaterials.Add(Material5);
		DefaultMaterial = DefaultMaterial1;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Material Saver - [ Status - Material Loading Error - Aborted ]"));
		return false;
	}

	ShowLogs = ShowOutputLogs;

	SharedMaterialMap.Empty();

	return true;
}

/**
 * Clean up after play has ended
 */
void FCPMaterialHelper::CleanUp()
{
	CRNLib.FreeCRNLibrary();
}

/*/**
 * Is the materials parent valid
 * @param InName - Parent material name
 * @return Valid - true, Invalid - false
 #1#
bool FCPMaterialHelper::IsMaterialValid(const FString& InName)
{
	for (UMaterialInterface* ParentMaterial : ParentMaterials)
	{
		if (FStaticMaterial(ParentMaterial).MaterialSlotName == InName)
			return true;
	}

	return false;
}*/

// Validate material has correct parent
bool FCPMaterialHelper::IsMaterialValid(UMaterialInterface* InMaterial)
{
	UMaterial* Root = GetRootMaterial(InMaterial);
	const bool bIsValid = Root && ParentMaterials.Contains(Root);
	if (!bIsValid)
		UE_LOG(LogTemp, Warning, TEXT("VWCP - [ Material Parent Not Supported, Material Name: %s, Parent Material: %s ]"), *InMaterial->GetName(), *Root->GetName());
	return bIsValid;	
}

// Validate materials have correct parent
bool FCPMaterialHelper::ValidateMaterials(TArray<UMaterialInterface*> InMaterials)
{
	bool bIsValid = true;
	for (UMaterialInterface* Material : InMaterials)
	{
		if (!IsMaterialValid(Material))
			bIsValid = false;
	}
	return bIsValid;
}

// Get the root material from the material
UMaterial* FCPMaterialHelper::GetRootMaterial(UMaterialInterface* InMaterial)
{
	if (!InMaterial)
	{
		return nullptr;
	}

	UMaterialInterface* Current = InMaterial;

	while (UMaterialInstance* MI = Cast<UMaterialInstance>(Current))
	{
		if (!MI->Parent)
		{
			break;
		}

		Current = MI->Parent;
	}

	return Cast<UMaterial>(Current);
}

/**
 * Add shared material with its material data
 * @param InMaterial - Material interface
 * @param InMaterialData - Material data
 */
void FCPMaterialHelper::AddMaterial(UMaterialInterface* InMaterial, const FMPMaterial& InMaterialData)
{
	SharedMaterialMap.Add(InMaterial, InMaterialData);
}

// Get material item
bool FCPMaterialHelper::GetMaterialItem(const UMaterialInterface* InMaterial, FMPMaterial& OutMaterialItem)
{
	if (const FMPMaterial* Found = SharedMaterialMap.Find(InMaterial))
	{
		OutMaterialItem = *Found;
		return true;
	}
	return false;
}

// Get the shared material items
TArray<FMPMaterial> FCPMaterialHelper::GetMaterialItems()
{
	TArray<FMPMaterial> MaterialItems;
	SharedMaterialMap.GenerateValueArray(MaterialItems);
	return MaterialItems;
}

/**
 * Serialize material
 * @param InMaterial - Material interface
 * @param OutMaterial - Material properties
 */
void FCPMaterialHelper::SerializeMaterial(UMaterialInterface* InMaterial, FMPMaterial& OutMaterial)
{
	UMaterialInstance* MatInst = Cast<UMaterialInstance>(InMaterial);

	// Create a material identifier (guid)
	OutMaterial.id = FGuid::NewGuid();

	// Set the material name
	OutMaterial.name = FName(InMaterial->GetName());

	// Set the parent material name
	OutMaterial.parent = FName(InMaterial->GetMaterial()->GetName());

	// Only overriden properties are serialized

	// Scalar Parameters
	for (const auto& Param : MatInst->ScalarParameterValues)
		OutMaterial.scalars.Add(FMPScalar(Param.ParameterInfo.Name, Param.ParameterValue));

	// Vector Parameters
	for (const auto& Param : MatInst->VectorParameterValues)
		OutMaterial.vectors.Add(FMPLinearColor(Param.ParameterInfo.Name, Param.ParameterValue));

	// Textures
	for (const auto& Param : MatInst->TextureParameterValues)
	{
		if (UTexture2D* Texture2D = Cast<UTexture2D>(Param.ParameterValue))
		{
			// Get the textures asset resource GUID
			FGuid TextureID = Texture2D->GetLightingGuid();
			FName TexturePath = FName(Texture2D->GetPathName());

			FMPTexture TextureItem = FMPTexture(
				TextureID,
				Param.ParameterInfo.Name,
				FName(Texture2D->GetName()),
				Texture2D->GetSizeX(),
				Texture2D->GetSizeY(),
				!Texture2D->SRGB,
				FName(CRNLib.PixelFormatToString(Texture2D->GetPixelFormat())));
			
			OutMaterial.textures.Add(TextureItem);
			
			FName Meta = Texture2D->SRGB ? TEXT("0") : TEXT("1");
			
			FPCacheHelper::AddAssetRef(FMPAssetRef(TextureID, TextureItem.name, FPAssetType::Texture, TexturePath, Meta));
		}
	}
	
	if (ShowLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("VWCP - [ Material Serialized, Material Name: %s  Material Parent: %s, Material ID: %s ]"),
		       *InMaterial->GetName(), *InMaterial->GetMaterial()->GetName(), *OutMaterial.id.ToString());
	}
}

// Serialize the texture
void FCPMaterialHelper::SerializeTexture(const FGuid& InTextureID, const FName& InTextureName, const FName& InTexturePath)
{
	UTexture2D* Texture2D = FPCacheHelper::LoadTextureFromAssetRegistry(InTexturePath.ToString());

	if (Texture2D == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Texture asset not found, Texture ID: %s ]"), *InTextureID.ToString());
		return;
	}

	// Otherwise create the texture asset
	TArray<uint8> CRNArray = CRNLib.CrunchTexture(Texture2D, Texture2D->SRGB, 128, true, false);

	if (!CRNArray.IsEmpty())
	{
		FPCacheHelper::SaveTextureAsset(InTextureID, CRNArray);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("VWCP - [ Texture Not Created (data is empty), Asset ID: %s ]"), *InTextureID.ToString());
	}

	if (ShowLogs)
		UE_LOG(LogTemp, Log, TEXT("VWCP - [ Texture Created, Name: %s, Asset ID: %s ]"), *InTextureName.ToString(), *InTextureID.ToString());
}

/**
 * Serialize the decal component
 * @param InComponent - Decal component
 * @param OutActorItem - Actor item
 * @param OutMaterialItems - Array of material items 
 * @return Success - true, Fail - false
 */
bool FCPMaterialHelper::SerializeDecalComponent(const UDecalComponent* InComponent, FMPActor& OutActorItem, TArray<FMPMaterial>& OutMaterialItems)
{
	FMPDecalComponent DecalComponent;

	DecalComponent.material_ref = {};
	DecalComponent.size = InComponent->DecalSize;
	DecalComponent.color = FMPLinearColor("Color", InComponent->DecalColor);
	DecalComponent.fade_screen_size = InComponent->FadeScreenSize;
	DecalComponent.fade_in_start_delay = InComponent->FadeInStartDelay;
	DecalComponent.fade_in_duration = InComponent->FadeInDuration;
	DecalComponent.fade_out_start_delay = InComponent->FadeStartDelay;
	DecalComponent.fade_out_duration = InComponent->FadeDuration;
	DecalComponent.destroy_after_fade_out = InComponent->bDestroyOwnerAfterFade;

	UMaterialInterface* DecalMaterial = InComponent->GetDecalMaterial();
	FMPMaterial MaterialItem;
	if (!GetMaterialItem(DecalMaterial, MaterialItem))
	{
		// Serialize the material
		SerializeMaterial(DecalMaterial, MaterialItem);

		// Add the shared material
		AddMaterial(DecalMaterial, MaterialItem);
	}
	
	DecalComponent.material_ref = FMPAssetRef(MaterialItem.id, MaterialItem.name, FPAssetType::Material, FName(DecalMaterial->GetPathName()), NAME_None);

	OutActorItem.decal_comp = DecalComponent;
	
	FCPStatsHelper::TotalDecals++;

	return true;
}
