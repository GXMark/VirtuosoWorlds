
#include "Asset/Agent/VMaterialAgent.h"

// Initialize material manager
bool UVMaterialAgent::Initialize(bool InServerMode)
{
	if (!InServerMode)
	{
#if WITH_CLIENT_CODE
		// Load the materials
		UMaterialInterface* Material1 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/MasterMaterial/Materials/MM_Opaque")));
		UMaterialInterface* Material2 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/MasterMaterial/Materials/MM_Opaque_ARMS")));
		UMaterialInterface* Material3 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/MasterMaterial/Materials/MM_GlassTT")));
		UMaterialInterface* Material4 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/MasterMaterial/Materials/MM_Glass_Ornate")));
		UMaterialInterface* Material5 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/MasterMaterial/Materials/MM_Decal")));
		UMaterialInterface* DefaultMaterial1 = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("/Game/MasterMaterial/Materials/DefaultMaterial")));

		if (Material1 && Material2 && Material3 && Material4 && Material5 && DefaultMaterial1)
		{
			ParentMaterialMap.Add(TEXT("MM_Opaque"), Material1);
			ParentMaterialMap.Add(TEXT("MM_Opaque_ARMS"), Material2);
			ParentMaterialMap.Add(TEXT("MM_GlassTT"), Material3);
			ParentMaterialMap.Add(TEXT("MM_Glass_Ornate"), Material4);
			ParentMaterialMap.Add(TEXT("MM_Decal"), Material5);
			DefaultMaterial = DefaultMaterial1;
			UE_LOG(LogTemp, Log, TEXT("Material Comp - Initialize [ Material Templated Loaded Successfully ]"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Material Comp - Initialize [ Material Templated Failed To Load ]"));
			return false;
		}
#endif
	}

	MaterialMap.Empty();

	return true;
}

// Is the material cached?
bool UVMaterialAgent::IsMaterialCached(const FGuid& InMaterialID) const
{
	return MaterialMap.Contains(InMaterialID);
}

// Remove all materials in shared-memory
void UVMaterialAgent::RemoveAll()
{
	MaterialMap.Empty();
}

// Get material asset identifiers
TArray<FGuid> UVMaterialAgent::GetMaterialIDs() const
{
	TArray<FGuid> MaterialAssetIDs;
	MaterialMap.GenerateKeyArray(MaterialAssetIDs);
	return MaterialAssetIDs;
}


// Set the material items
void UVMaterialAgent::AddMaterialItems(const TArray<FVMMaterial>& InMaterialItems)
{
	for (const FVMMaterial& MaterialItem : InMaterialItems)
		MaterialItemMap.Add(MaterialItem.id, MaterialItem);
}

// Set the material item
void UVMaterialAgent::AddMaterialItem(const FVMMaterial& InMaterialItem)
{
	MaterialItemMap.Add(InMaterialItem.id, InMaterialItem);
}

// Get texture refs for an array of material items
/*TArray<FMTextureRef> UMaterialAssets::GetTextureAssetRefs(const TArray<FMMaterialRef>& InMaterialRefs)
{
	TArray<FMTextureRef> OutTextureAssetRefs;

	TArray<FGuid> TextureIDs;

	for (FMMaterialRef MaterialRef : InMaterialRefs)
	{
		if (MaterialItemMap.Contains(MaterialRef.id))
		{
			for (FMTexture TextureItem : MaterialItemMap[MaterialRef.id].textures)
			{
				if (!TextureIDs.Contains(TextureItem.id))
				{
					OutTextureAssetRefs.Add(FMTextureRef(
						TextureItem.id, 
						TextureItem.name, 
						TextureItem.path,
						TextureItem.linear));
					TextureIDs.Add(TextureItem.id);
				}
			}
		}
	}
	return OutTextureAssetRefs;
}*/

// Deserialize the material item to a material instance
void UVMaterialAgent::DeserializeMaterialItem(const FVMMaterial& InMaterialItem, UVTextureAgent* InTextureAssets)
{
	// Get the material instance
	UMaterialInstanceDynamic* MaterialInstance = GetMaterial(InMaterialItem.id);

	// If the material instance does not exist then create it
	if (!MaterialInstance)
	{
		// Find parent material
		UMaterialInterface* ParentMaterial;
		if (!GetParentMaterial(InMaterialItem.parent.ToString(), ParentMaterial))
		{
			UE_LOG(LogTemp, Warning, TEXT("Material Comp - Deserialize Material Item  [ Parent Material Not Found - Using Default Material ]"));
			SetMaterial(InMaterialItem.id, Cast<UMaterialInstanceDynamic>(DefaultMaterial));
		}

		// Create a new instance material if not exists
		MaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterial, this);

		if (!MaterialInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Material Comp - Deserialize Material Item  [ Material Instance Not Created, Using Default Material ]"));
			SetMaterial(InMaterialItem.id, Cast<UMaterialInstanceDynamic>(DefaultMaterial));
		}
	}

	// Assign material item properties to the material instance
	for (const FVMScalar& ScalarItem : InMaterialItem.scalars)
	{
		MaterialInstance->SetScalarParameterValue(ScalarItem.name, ScalarItem.value);
		UE_LOG(LogTemp, Log, TEXT("Material Comp - Deserialize Material Item [ Scalar Item: %s, Value: %f ]"), *ScalarItem.name.ToString(), ScalarItem.value);
	}

	for (const FVMLinearColor& VectorItem : InMaterialItem.vectors)
	{
		MaterialInstance->SetVectorParameterValue(VectorItem.name, VectorItem.color);
		UE_LOG(LogTemp, Log, TEXT("Material Comp - Deserialize Material Item [ Vector Item: %s, Value: %s ]"), *VectorItem.name.ToString(), *VectorItem.color.ToString());
	}

	for (const FVMTexture& TextureItem : InMaterialItem.textures)
	{
		if (UTexture2D* Texture = InTextureAssets->GetTexture(TextureItem.asset_ref.id))
			MaterialInstance->SetTextureParameterValue(TextureItem.property, Texture);
	}

	/* DEBUG MATERIAL PROPERTIES
	UE_LOG(LogTemp, Log, TEXT("Material Instance Parameters:"));
	TArray<FMaterialParameterInfo> ScalarParams;
	TArray<FGuid> ScalarGuids;
	MaterialInstance->GetAllScalarParameterInfo(ScalarParams, ScalarGuids);
	for (auto& Param : ScalarParams)
	{
		float Val;
		MaterialInstance->GetScalarParameterValue(Param, Val);
		UE_LOG(LogTemp, Log, TEXT("Debug Scalar: %s = %f"), *Param.Name.ToString(), Val);
	}*/

	// Update the material instance
	SetMaterial(InMaterialItem.id, MaterialInstance);
}

// Get the material item
bool UVMaterialAgent::GetMaterialItem(const FGuid& InMaterialID, FVMMaterial& OutMaterialItem)
{
	if (MaterialItemMap.Contains(InMaterialID))
	{
		OutMaterialItem = MaterialItemMap[InMaterialID];
		return true;
	}
	return false;
}

// Get parent material by material name
bool UVMaterialAgent::GetParentMaterial(const FString& InMaterialName, UMaterialInterface*& OutParentMaterial)
{
	if (ParentMaterialMap.Contains(InMaterialName))
	{
		OutParentMaterial = ParentMaterialMap[InMaterialName];
		return true;
	}
	return false;
}

// Set material by material identifier
void UVMaterialAgent::SetMaterial(const FGuid& InKey, UMaterialInstanceDynamic* InMaterialInstance)
{
	if (!InMaterialInstance)
		return;

	MaterialMap.Add(InKey, InMaterialInstance);
}

// Get material by material identifier
UMaterialInstanceDynamic* UVMaterialAgent::GetMaterial(const FGuid& InMaterialID)
{
	if (MaterialMap.Contains(InMaterialID))
		return MaterialMap[InMaterialID];
	return nullptr;
}

// Create material instance by material identifier
UMaterialInstanceDynamic* UVMaterialAgent::CreateMaterial(const FGuid& InMaterialID, const FName& InParent)
{
	// Safety first - return the material instance if its found
	if (MaterialMap.Contains(InMaterialID))
		return MaterialMap[InMaterialID];

	// Find parent material
	UMaterialInterface* ParentMaterial;
	if (!GetParentMaterial(InParent.ToString(), ParentMaterial))
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Comp - Create Material Instance  [ Parent Material Not Found - Using Default Material ]"));
		return Cast<UMaterialInstanceDynamic>(DefaultMaterial);
	}

	// Create a new instance material if not exists
	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterial, this);

	if (!MaterialInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Comp - Create Material Instance  [ Material Instance Not Created, Using Default Material ]"));
		return Cast<UMaterialInstanceDynamic>(DefaultMaterial);
	}

	return MaterialInstance;
}

