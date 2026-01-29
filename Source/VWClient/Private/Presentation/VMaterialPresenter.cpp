#include "Presentation/VMaterialPresenter.h"

#include "Components/StaticMeshComponent.h"
#include "Subsystem/VAssetManager.h"

void UVMaterialPresenter::Initialize(UVAssetManager* InAssetManager)
{
	AssetManager = InAssetManager;
}

void UVMaterialPresenter::ApplyMaterialsAsync(
	const FGuid& InItemId,
	UStaticMeshComponent* MeshComp,
	const TArray<FVMGuidNet>& InMaterialIds)
{
	if (!MeshComp || !AssetManager)
	{
		return;
	}

	MeshByItemId.Add(InItemId, MeshComp);

	TArray<FGuid> Desired;
	Desired.Reserve(InMaterialIds.Num());
	for (const FVMGuidNet& NetId : InMaterialIds)
	{
		Desired.Add(NetId.Value);
	}

	if (const TArray<FGuid>* Prev = RequestedMaterialsByItemId.Find(InItemId))
	{
		if (Prev->Num() == Desired.Num())
		{
			bool bSame = true;
			for (int32 i = 0; i < Desired.Num(); ++i)
			{
				if ((*Prev)[i] != Desired[i])
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

	RequestedMaterialsByItemId.Add(InItemId, Desired);

	for (int32 SlotIndex = 0; SlotIndex < Desired.Num(); ++SlotIndex)
	{
		const FGuid MatId = Desired[SlotIndex];
		if (!MatId.IsValid())
		{
			continue;
		}

		AssetManager->RequestMaterialInstanceAsync(
			MatId,
			FVOnMaterialInstanceLoaded::CreateLambda(
				[this, InItemId, MatId, SlotIndex](UMaterialInstanceDynamic* LoadedMID)
				{
					if (!LoadedMID)
					{
						return;
					}

					const TArray<FGuid>* CurrentDesired = RequestedMaterialsByItemId.Find(InItemId);
					if (!CurrentDesired || !CurrentDesired->IsValidIndex(SlotIndex) || (*CurrentDesired)[SlotIndex] != MatId)
					{
						return;
					}

					if (TWeakObjectPtr<UStaticMeshComponent>* MeshPtr = MeshByItemId.Find(InItemId))
					{
						if (MeshPtr->IsValid())
						{
							MeshPtr->Get()->SetMaterial(SlotIndex, LoadedMID);
						}
					}
				}));
	}
}

void UVMaterialPresenter::ForgetItem(const FGuid& InItemId)
{
	MeshByItemId.Remove(InItemId);
	RequestedMaterialsByItemId.Remove(InItemId);
}
