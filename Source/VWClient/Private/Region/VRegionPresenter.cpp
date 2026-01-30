#include "Region/VRegionPresenter.h"

#include "Presentation/VCollisionPresenter.h"
#include "Presentation/VDecalPresenter.h"
#include "Presentation/VLightPresenter.h"
#include "Presentation/VMaterialPresenter.h"
#include "Presentation/VMeshPresenter.h"
#include "Presentation/VSpatialItemComponentRegistry.h"
#include "Materials/MaterialInterface.h"
#include "Components/SceneComponent.h"

void UVRegionPresenter::Initialize(
	UVMeshPresenter* InMeshPresenter,
	UVMaterialPresenter* InMaterialPresenter,
	UVLightPresenter* InLightPresenter,
	UVDecalPresenter* InDecalPresenter,
	UVCollisionPresenter* InCollisionPresenter,
	UVSpatialItemComponentRegistry* InItemRegistry)
{
	MeshPresenter = InMeshPresenter;
	MaterialPresenter = InMaterialPresenter;
	LightPresenter = InLightPresenter;
	DecalPresenter = InDecalPresenter;
	CollisionPresenter = InCollisionPresenter;
	ItemRegistry = InItemRegistry;
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
	if (LightPresenter.IsValid())
	{
		LightPresenter->DestroyItem(ItemId);
	}
	if (DecalPresenter.IsValid())
	{
		DecalPresenter->DestroyItem(ItemId);
	}
	if (CollisionPresenter.IsValid())
	{
		CollisionPresenter->OnItemRemoved(ItemId);
	}
	if (ItemRegistry.IsValid())
	{
		ItemRegistry->DestroyItemRoot(ItemId);
	}
	if (MaterialPresenter.IsValid())
	{
		MaterialPresenter->ForgetItem(ItemId);
	}
}

void UVRegionPresenter::PresentBundle(const FResolvedItemBundle& Bundle)
{
	if (ItemRegistry.IsValid())
	{
		if (USceneComponent* ItemRoot = ItemRegistry->GetOrCreateItemRoot(Bundle.ItemId))
		{
			ItemRoot->SetWorldTransform(Bundle.WorldTransform);
		}
	}

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
				if (Material)
				{
					Materials.Add(Material);
				}
			}
			MaterialPresenter->ApplyMaterials(Bundle.ItemId, MeshComp, Materials);
		}
		break;
	}
	case ESpatialItemType::PointLight:
		if (LightPresenter.IsValid())
		{
			LightPresenter->PresentPointLightItem(Bundle.ItemId, Bundle.PointLightPayload, Bundle.WorldTransform);
		}
		break;
	case ESpatialItemType::SpotLight:
		if (LightPresenter.IsValid())
		{
			LightPresenter->PresentSpotLightItem(Bundle.ItemId, Bundle.SpotLightPayload, Bundle.WorldTransform);
		}
		break;
	case ESpatialItemType::Decal:
		if (DecalPresenter.IsValid())
		{
			UMaterialInterface* DecalMaterial = nullptr;
			if (Bundle.Materials.Num() > 0)
			{
				DecalMaterial = Bundle.Materials[0].Get();
			}
			DecalPresenter->PresentDecalItem(Bundle.ItemId, Bundle.DecalPayload, Bundle.WorldTransform, DecalMaterial);
		}
		break;
	default:
		break;
	}

	if (Bundle.bHasCollision && CollisionPresenter.IsValid())
	{
		TArray<FVMCollision> CollisionDefs;
		CollisionDefs.Add(Bundle.CollisionData);
		CollisionPresenter->SubmitCollisionDefs(CollisionDefs);
		CollisionPresenter->PresentCollision(Bundle.ItemId, Bundle.WorldTransform, Bundle.CollisionData.id);
	}
}
