#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VMaterialPresenterApplier.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class UVMaterialResolver;

UCLASS()
class VWCLIENT_API UVMaterialPresenterApplier : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UVMaterialResolver* InResolver);
	void ApplyMaterials(UStaticMeshComponent* MeshComp, const TArray<uint32>& MaterialIdsBySlot);
	void ApplyMaterialsForSlots(
		UStaticMeshComponent* MeshComp,
		const TArray<uint32>& MaterialIdsBySlot,
		const TArray<int32>& SlotsToApply);
	void ForgetMeshComponent(UStaticMeshComponent* MeshComp);

private:
	struct FMaterialApplyState
	{
		TArray<uint32> MaterialIdsBySlot;
		int32 Revision = 0;
	};

	void HandleMaterialResolved(
		uint32 MaterialId,
		UMaterialInterface* Material,
		TWeakObjectPtr<UStaticMeshComponent> MeshComp,
		int32 Revision,
		int32 SlotIndex);

	void ApplyMaterialsInternal(
		UStaticMeshComponent* MeshComp,
		const TArray<uint32>& MaterialIdsBySlot,
		const TArray<int32>* SlotsToApply);

	TMap<TWeakObjectPtr<UStaticMeshComponent>, FMaterialApplyState> StateByComponent;
	TWeakObjectPtr<UVMaterialResolver> Resolver;
};
