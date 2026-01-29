#pragma once

#include "CoreMinimal.h"
#include "Model/Package/VMTexture.h"
#include "VTextureItem.generated.h"

USTRUCT()
struct VWSHARED_API FVTextureItem
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid MaterialID;
	
	UPROPERTY()
	FVMTexture Item;
	
	FVTextureItem() {}
	FVTextureItem(const FGuid& InMaterialID, const FVMTexture& InTextureItem) : MaterialID(InMaterialID), Item(InTextureItem) {}
};
