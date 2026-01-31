#include "Region/VRegionPresenter.h"

#include "Presentation/VMaterialPresenter.h"
#include "Presentation/VMeshPresenter.h"
#include "Materials/MaterialInterface.h"

void UVRegionPresenter::Initialize(
	UVMeshPresenter* InMeshPresenter,
	UVMaterialPresenter* InMaterialPresenter)
{
	MeshPresenter = InMeshPresenter;
	MaterialPresenter = InMaterialPresenter;
}

void UVRegionPresenter::Commit(const TArray<FResolvedItemBundle>& Bundles)
{
	if (Bundles.Num() == 0)
	{
		return;
	}

	for (const FResolvedItemBundle& Bundle : Bundles)
	{
		PresentBundle(Bundle);
	}
}

void UVRegionPresenter::ReleaseItem(const FGuid& ItemId)
{
	if (MeshPresenter.IsValid())
	{
		MeshPresenter->DestroyItem(ItemId);
	}
	if (MaterialPresenter.IsValid())
	{
		MaterialPresenter->ForgetItem(ItemId);
	}
}

void UVRegionPresenter::PresentBundle(const FResolvedItemBundle& Bundle)
{
	switch (Bundle.ItemType)
	{
	case ESpatialItemType::Mesh:
	{
		UStaticMeshComponent* MeshComp = nullptr;
		if (MeshPresenter.IsValid())
		{
			MeshComp = MeshPresenter->PresentMeshItemWithAsset(
				Bundle.ItemId,
				Bundle.MeshPayload,
				Bundle.WorldTransform,
				Bundle.ParentId,
				Bundle.MeshAsset.Get());
		}

		if (MaterialPresenter.IsValid() && MeshComp)
		{
			TArray<UMaterialInterface*> Materials;
			Materials.Reserve(Bundle.Materials.Num());
			for (const TObjectPtr<UMaterialInterface>& Material : Bundle.Materials)
			{
				Materials.Add(Material);
			}
			MaterialPresenter->ApplyMaterials(Bundle.ItemId, MeshComp, Materials);
		}
		break;
	}
	case ESpatialItemType::PointLight:
		break;
	case ESpatialItemType::SpotLight:
		break;
	case ESpatialItemType::Decal:
		break;
	default:
		break;
	}

}
