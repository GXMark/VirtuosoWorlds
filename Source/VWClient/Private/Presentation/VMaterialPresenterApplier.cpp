#include "Presentation/VMaterialPresenterApplier.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Presentation/VMaterialResolver.h"

void UVMaterialPresenterApplier::Initialize(UVMaterialResolver* InResolver)
{
	Resolver = InResolver;
}

void UVMaterialPresenterApplier::ApplyMaterials(UStaticMeshComponent* MeshComp, const TArray<uint32>& MaterialIdsBySlot)
{
	if (!MeshComp || !Resolver.IsValid())
	{
		return;
	}

	FMaterialApplyState& State = StateByComponent.FindOrAdd(MeshComp);
	State.MaterialIdsBySlot = MaterialIdsBySlot;
	State.Revision += 1;

	const int32 SlotCount = MeshComp->GetNumMaterials();
	if (SlotCount <= 0)
	{
		return;
	}

	const int32 Revision = State.Revision;
	const TWeakObjectPtr<UStaticMeshComponent> WeakComp(MeshComp);

	for (int32 SlotIndex = 0; SlotIndex < MaterialIdsBySlot.Num(); ++SlotIndex)
	{
		const uint32 MaterialId = MaterialIdsBySlot[SlotIndex];
		if (MaterialId == 0)
		{
			continue;
		}

		const int32 ClampedSlot = FMath::Clamp(SlotIndex, 0, SlotCount - 1);

		Resolver->ResolveMaterial(
			MaterialId,
			FVOnMaterialResolved::CreateUObject(
				this,
				&UVMaterialPresenterApplier::HandleMaterialResolved,
				WeakComp,
				Revision,
				ClampedSlot));
	}
}

void UVMaterialPresenterApplier::ForgetMeshComponent(UStaticMeshComponent* MeshComp)
{
	if (!MeshComp)
	{
		return;
	}

	StateByComponent.Remove(MeshComp);
}

void UVMaterialPresenterApplier::HandleMaterialResolved(
	uint32 MaterialId,
	UMaterialInterface* Material,
	TWeakObjectPtr<UStaticMeshComponent> MeshComp,
	int32 Revision,
	int32 SlotIndex)
{
	if (!MeshComp.IsValid() || !Material)
	{
		return;
	}

	FMaterialApplyState* State = StateByComponent.Find(MeshComp);
	if (!State || State->Revision != Revision)
	{
		return;
	}

	const int32 SlotCount = MeshComp->GetNumMaterials();
	if (SlotCount <= 0)
	{
		return;
	}

	const int32 ClampedSlot = FMath::Clamp(SlotIndex, 0, SlotCount - 1);
	MeshComp->SetMaterial(ClampedSlot, Material);
}
