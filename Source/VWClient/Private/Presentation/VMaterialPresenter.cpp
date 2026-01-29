#include "Presentation/VMaterialPresenter.h"

#include "Components/StaticMeshComponent.h"
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

	if (const TArray<UMaterialInterface*>* Prev = AppliedMaterialsByItemId.Find(InItemId))
	{
		if (Prev->Num() == InMaterials.Num())
		{
			bool bSame = true;
			for (int32 i = 0; i < InMaterials.Num(); ++i)
			{
				if ((*Prev)[i] != InMaterials[i])
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

	AppliedMaterialsByItemId.Add(InItemId, InMaterials);

	for (int32 SlotIndex = 0; SlotIndex < InMaterials.Num(); ++SlotIndex)
	{
		UMaterialInterface* Mat = InMaterials[SlotIndex];
		if (!Mat)
		{
			continue;
		}
		MeshComp->SetMaterial(SlotIndex, Mat);
	}
}

void UVMaterialPresenter::ForgetItem(const FGuid& InItemId)
{
	MeshByItemId.Remove(InItemId);
	AppliedMaterialsByItemId.Remove(InItemId);
}
