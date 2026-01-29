#pragma once
#include "CoreMinimal.h"
#include "VTextureAgent.h"
#include "Engine/StaticMesh.h"
#include "Model/Package/VMMaterial.h"
#include "VMaterialAgent.generated.h"

UCLASS()
class VWCLIENT_API UVMaterialAgent : public UObject
{
	GENERATED_BODY()
public:
	bool Initialize(bool InServerMode);

	// Cache
	bool IsMaterialCached(const FGuid& InMaterialID) const;
	void RemoveAll();
	TArray<FGuid> GetMaterialIDs() const;
	//TArray<FMTextureRef> GetTextureAssetRefs(const TArray<FMMaterialRef>& InMaterialRefs);

	// Serializer
	void DeserializeMaterialItem(const FVMMaterial& InMaterialItem, UVTextureAgent* InTextureAssets);

	// Material Item
	void AddMaterialItems(const TArray<FVMMaterial>& InMaterialItems);
	void AddMaterialItem(const FVMMaterial& InMaterialItem);
	bool GetMaterialItem(const FGuid& InMaterialID, FVMMaterial& OutMaterialItem);
	
	// Material Instance
	bool GetParentMaterial(const FString& InMaterialName, UMaterialInterface*& OutParentMaterial);
	void SetMaterial(const FGuid& InKey, UMaterialInstanceDynamic* InMaterialInstance);
	UMaterialInstanceDynamic* GetMaterial(const FGuid& InKey);
	UMaterialInstanceDynamic* CreateMaterial(const FGuid& InMaterialID, const FName& InParent);

private:
	// Material Items
	UPROPERTY()
	TMap<FGuid, FVMMaterial> MaterialItemMap;
	
	// Material Parents
	UPROPERTY()
	TMap<FString, UMaterialInterface*> ParentMaterialMap;
	
	UPROPERTY()
	UMaterialInterface* DefaultMaterial;
	
	// Material Instances
	UPROPERTY()
	TMap<FGuid, UMaterialInstanceDynamic*> MaterialMap;
};
