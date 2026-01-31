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
	ApplyMaterialsInternal(MeshComp, MaterialIdsBySlot, nullptr);
}

void UVMaterialPresenterApplier::ApplyMaterialsForSlots(
	UStaticMeshComponent* MeshComp,
	const TArray<uint32>& MaterialIdsBySlot,
	const TArray<int32>& SlotsToApply)
{
	ApplyMaterialsInternal(MeshComp, MaterialIdsBySlot, &SlotsToApply);
}

void UVMaterialPresenterApplier::ApplyMaterialsInternal(
	UStaticMeshComponent* MeshComp,
	const TArray<uint32>& MaterialIdsBySlot,
	const TArray<int32>* SlotsToApply)
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

	const int32 SlotUpperBound = MaterialIdsBySlot.Num();
	const TArray<int32>* Slots = SlotsToApply;

	auto ApplySlot = [&](int32 SlotIndex)
	{
		if (SlotIndex < 0 || SlotIndex >= SlotUpperBound)
		{
			return;
		}

		const uint32 MaterialId = MaterialIdsBySlot[SlotIndex];
		if (MaterialId == 0)
		{
			return;
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
	};

	if (Slots && Slots->Num() > 0)
	{
		for (const int32 SlotIndex : *Slots)
		{
			ApplySlot(SlotIndex);
		}
	}
	else
	{
		for (int32 SlotIndex = 0; SlotIndex < MaterialIdsBySlot.Num(); ++SlotIndex)
		{
			ApplySlot(SlotIndex);
		}
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
