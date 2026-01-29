#pragma once

#include "CoreMinimal.h"
#include "Model/Package/MPActor.h"
#include "Utility/CrunchUtility.h"
#include "Model/Package/MPMaterial.h"

class VW_CREATORS_PLUGIN_API FCPMaterialHelper
{
public:
	static bool Initialize(bool ShowLogs);
	static void CleanUp();
	static bool IsMaterialValid(UMaterialInterface* InMaterial);
	static bool ValidateMaterials(TArray<UMaterialInterface*> InMaterials);
	static void AddMaterial(UMaterialInterface* InMaterial, const FMPMaterial& InMaterialData);
	static bool GetMaterialItem(const UMaterialInterface* InMaterial, FMPMaterial& OutMaterialItem);
	static TArray<FMPMaterial> GetMaterialItems();
	static bool SerializeDecalComponent(const UDecalComponent* InComponent, FMPActor& OutActorItem, TArray<FMPMaterial>& OutMaterialItems);
	static void SerializeTexture(const FGuid& InTextureID, const FName& InTextureName, const FName& InTexturePath);
	static void SerializeMaterial(UMaterialInterface* InMaterial, FMPMaterial& OutMaterial);
private:
	static bool ShowLogs;
	static TMap<UMaterialInterface*, FMPMaterial> SharedMaterialMap;
	static FCPCrunchUtility CRNLib;
	static TArray<UMaterialInterface*> ParentMaterials;
	static UMaterialInterface* DefaultMaterial;
	
	static UMaterial* GetRootMaterial(UMaterialInterface* InMaterial);
};
