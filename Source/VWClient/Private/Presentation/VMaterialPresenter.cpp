#include "Presentation/VMaterialPresenter.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
void UVMaterialPresenter::Initialize()
{
}

void UVMaterialPresenter::ApplyMaterials(
	const FGuid& InItemId,
	UStaticMeshComponent* MeshComp,
	const TArray<UMaterialInterface*>& InMaterials)
{
	if (!MeshComp)
	{
		return;
	}

	MeshByItemId.Add(InItemId, MeshComp);

	TArray<UMaterialInterface*> ResolvedMaterials;
	ResolvedMaterials.Reserve(InMaterials.Num());
	for (UMaterialInterface* Mat : InMaterials)
	{
		ResolvedMaterials.Add(Mat ? Mat : UMaterial::GetDefaultMaterial(MD_Surface));
	}

	if (const TArray<UMaterialInterface*>* Prev = AppliedMaterialsByItemId.Find(InItemId))
	{
		if (Prev->Num() == ResolvedMaterials.Num())
		{
			bool bSame = true;
			for (int32 i = 0; i < ResolvedMaterials.Num(); ++i)
			{
				if ((*Prev)[i] != ResolvedMaterials[i])
				{
					bSame = false;
					break;
				}
			}
			if (bSame)
			{
				return;
			}
		}
	}

	AppliedMaterialsByItemId.Add(InItemId, ResolvedMaterials);

	for (int32 SlotIndex = 0; SlotIndex < ResolvedMaterials.Num(); ++SlotIndex)
	{
		MeshComp->SetMaterial(SlotIndex, ResolvedMaterials[SlotIndex]);
	}
}

void UVMaterialPresenter::ForgetItem(const FGuid& InItemId)
{
	MeshByItemId.Remove(InItemId);
	AppliedMaterialsByItemId.Remove(InItemId);
}
