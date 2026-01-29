#include "Presentation/VDecalPresenter.h"

#include "Components/DecalComponent.h"
#include "Subsystem/VAssetManager.h"

void UVDecalPresenter::Initialize(AActor* InOwner, USceneComponent* InPresentationRoot, UVAssetManager* InAssetManager)
{
	PresentationOwner = InOwner;
	PresentationRoot = InPresentationRoot;
	AssetManager = InAssetManager;
}

void UVDecalPresenter::PresentDecalItem(
	const FGuid& InItemId,
	const FVMDecalComponentNet& InDecalData,
	const FTransform& InWorldTransform)
{
	if (!PresentationOwner.IsValid() || !PresentationRoot.IsValid() || !AssetManager)
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

	const FGuid MaterialId = InDecalData.material_id.Value;
	if (!MaterialId.IsValid())
	{
		return;
	}

	if (const FGuid* CurrentDesired = RequestedMaterialsByItemId.Find(InItemId))
	{
		if (*CurrentDesired == MaterialId)
		{
			return;
		}
	}

	RequestedMaterialsByItemId.Add(InItemId, MaterialId);

	AssetManager->RequestMaterialInstanceAsync(
		MaterialId,
		FVOnMaterialInstanceLoaded::CreateLambda(
			[this, InItemId, MaterialId](UMaterialInstanceDynamic* LoadedMID)
			{
				if (!LoadedMID)
				{
					return;
				}

				const FGuid* CurrentDesired = RequestedMaterialsByItemId.Find(InItemId);
				if (!CurrentDesired || *CurrentDesired != MaterialId)
				{
					return;
				}

				if (UDecalComponent* Decal = SpawnedComponents.FindRef(InItemId))
				{
					Decal->SetDecalMaterial(LoadedMID);
				}
			}));
}

void UVDecalPresenter::DestroyItem(const FGuid& InItemId)
{
	if (UDecalComponent* Comp = SpawnedComponents.FindRef(InItemId))
	{
		Comp->DestroyComponent();
	}
	SpawnedComponents.Remove(InItemId);
	RequestedMaterialsByItemId.Remove(InItemId);
}
