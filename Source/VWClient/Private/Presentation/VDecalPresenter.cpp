#include "Presentation/VDecalPresenter.h"

#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

void UVDecalPresenter::Initialize(AActor* InOwner, USceneComponent* InPresentationRoot)
{
	PresentationOwner = InOwner;
	PresentationRoot = InPresentationRoot;
}

void UVDecalPresenter::PresentDecalItem(
	const FGuid& InItemId,
	const FVMDecalComponentNet& InDecalData,
	const FTransform& InWorldTransform,
	UMaterialInterface* InMaterial)
{
	if (!PresentationOwner.IsValid() || !PresentationRoot.IsValid())
	{
		return;
	}

	UDecalComponent* DecalComp = SpawnedComponents.FindRef(InItemId);
	if (!DecalComp)
	{
		DecalComp = NewObject<UDecalComponent>(PresentationOwner.Get());
		DecalComp->AttachToComponent(PresentationRoot.Get(), FAttachmentTransformRules::KeepWorldTransform);
		DecalComp->RegisterComponent();
		SpawnedComponents.Add(InItemId, DecalComp);
	}

	DecalComp->SetWorldTransform(InWorldTransform);
	DecalComp->DecalSize = FVector(
		static_cast<float>(InDecalData.size.X),
		static_cast<float>(InDecalData.size.Y),
		static_cast<float>(InDecalData.size.Z));
	DecalComp->SetDecalColor(InDecalData.color.color);
	DecalComp->SetFadeScreenSize(InDecalData.fade_screen_size);
	DecalComp->SetFadeIn(InDecalData.fade_in_start_delay, InDecalData.fade_in_duration);
	DecalComp->SetFadeOut(
		InDecalData.fade_out_start_delay,
		InDecalData.fade_out_duration,
		InDecalData.destroy_after_fade_out);

	if (!InMaterial)
	{
		return;
	}

	DecalComp->SetDecalMaterial(InMaterial);
}

UDecalComponent* UVDecalPresenter::FindDecalComponent(const FGuid& InItemId) const
{
	return SpawnedComponents.FindRef(InItemId);
}

void UVDecalPresenter::DestroyItem(const FGuid& InItemId)
{
	if (UDecalComponent* Comp = SpawnedComponents.FindRef(InItemId))
	{
		Comp->DestroyComponent();
	}
	SpawnedComponents.Remove(InItemId);
}
