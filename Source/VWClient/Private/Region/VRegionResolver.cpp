#include "Region/VRegionResolver.h"

#include "Region/VRegionClientBridge.h"
#include "Subsystem/VAssetManager.h"
#include "Asset/Agent/VMaterialAgent.h"
#include "Asset/Agent/VMeshAgent.h"
#include "Model/Constant/VConstants.h"

void UVRegionResolver::Initialize(
	URegionClientBridge* InRegionBridge,
	UVAssetManager* InAssetManager)
{
	RegionBridge = InRegionBridge;
	AssetManager = InAssetManager;
}

void UVRegionResolver::OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items)
{
	for (const FVMSpatialItemNet& Item : Items)
	{
		const FGuid ItemId = Item.ItemID.Value;
		if (!ItemId.IsValid())
		{
			continue;
		}
		if (RemovedItemIds.Contains(ItemId) || IssuedItemIds.Contains(ItemId))
		{
			continue;
		}

		PendingSpatialData.Add(ItemId, Item);
		UpdateOrCreateInstance(Item);

		if (!SpatialOrder.Contains(ItemId))
		{
			SpatialOrder.Add(ItemId);
		}
	}

	IssueDependencyRequests();
}

void UVRegionResolver::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	if (AssetManager.IsValid())
	{
		AssetManager->SubmitMaterialItems(Materials);
	}

	for (const FVMMaterial& Mat : Materials)
	{
		if (!Mat.id.IsValid())
		{
			continue;
		}
		PendingMaterials.Add(Mat.id, Mat);
		RequestedMaterialIds.Remove(Mat.id);
	}

	for (TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
	{
		FResolvedSpatialInstance& Instance = Pair.Value;
		for (const FGuid& MatId : Instance.MaterialIds)
		{
			if (PendingMaterials.Contains(MatId))
			{
				UpdateTextureDependencies(Instance);
				break;
			}
		}
	}
	bMaterialRequestInFlight = false;
}

void UVRegionResolver::OnCollisionsBatchReceived(const TArray<FVMCollision>& Collisions)
{
	for (const FVMCollision& Col : Collisions)
	{
		if (!Col.id.IsValid())
		{
			continue;
		}
		PendingCollisions.Add(Col.id, Col);
		RequestedCollisionIds.Remove(Col.id);
	}

	bCollisionRequestInFlight = false;
}

void UVRegionResolver::OnSpatialItemRemoved(const FGuid& ItemId)
{
	RemovedItemIds.Add(ItemId);
	PendingSpatialData.Remove(ItemId);
	ResolvedInstances.Remove(ItemId);
	SpatialOrder.Remove(ItemId);
	IssuedItemIds.Remove(ItemId);
}

bool UVRegionResolver::IsBundleReady(const FGuid& ItemId) const
{
	if (RemovedItemIds.Contains(ItemId))
	{
		return false;
	}
	if (!PendingSpatialData.Contains(ItemId))
	{
		return false;
	}
	const FResolvedSpatialInstance* Instance = ResolvedInstances.Find(ItemId);
	if (!Instance || Instance->bIssued)
	{
		return false;
	}

	if (!CanRender(*Instance))
	{
		return false;
	}

	if (!CanActivate(*Instance))
	{
		return false;
	}

	return true;
}

void UVRegionResolver::GetResolvedBundles(int32 MaxResolvedBundlesPerTick, TArray<FResolvedItemBundle>& OutBundles)
{
	if (MaxResolvedBundlesPerTick <= 0)
	{
		return;
	}

	IssueDependencyRequests();

	TArray<FGuid> IssuedThisTick;
	for (const FGuid& ItemId : SpatialOrder)
	{
		if (OutBundles.Num() >= MaxResolvedBundlesPerTick)
		{
			break;
		}

		if (!IsBundleReady(ItemId))
		{
			continue;
		}

		const FVMSpatialItemNet* Item = PendingSpatialData.Find(ItemId);
		FResolvedSpatialInstance* Instance = ResolvedInstances.Find(ItemId);
		if (!Item || !Instance)
		{
			continue;
		}

		FResolvedItemBundle Bundle;
		Bundle.ItemId = ItemId;
		Bundle.ParentId = Item->ParentID.Value;
		Bundle.ItemType = Item->PayloadType;
		Bundle.WorldTransform = FTransform(
			Item->Transform.Rotation.ToQuat(),
			FVector(Item->Transform.Location),
			Item->Transform.bHasScale ? FVector(Item->Transform.Scale) : FVector(1.f, 1.f, 1.f));
		Bundle.MeshPayload = Item->MeshPayload;
		Bundle.PointLightPayload = Item->PointLightPayload;
		Bundle.SpotLightPayload = Item->SpotLightPayload;
		Bundle.DecalPayload = Item->DecalPayload;

		if (Item->PayloadType == ESpatialItemType::Mesh)
		{
			if (AssetManager.IsValid() && AssetManager->MeshAgent)
			{
				const FGuid MeshId = Item->MeshPayload.mesh_ref.id.Value;
				Bundle.MeshAsset = AssetManager->MeshAgent->GetMesh(MeshId);
			}
		}

		if (Item->PayloadType == ESpatialItemType::Mesh || Item->PayloadType == ESpatialItemType::Decal)
		{
			Bundle.Materials.Reserve(Instance->MaterialIds.Num());
			if (AssetManager.IsValid() && AssetManager->MaterialAgent)
			{
				for (const FGuid& MatId : Instance->MaterialIds)
				{
					if (UMaterialInterface* Material = AssetManager->MaterialAgent->GetMaterial(MatId))
					{
						Bundle.Materials.Add(Material);
					}
				}
			}
			Bundle.bTexturesReady = AreTexturesReady(*Instance);
		}

		if (Instance->CollisionId.IsValid())
		{
			if (const FVMCollision* Collision = PendingCollisions.Find(Instance->CollisionId))
			{
				Bundle.bHasCollision = true;
				Bundle.CollisionData = *Collision;
			}
		}

		OutBundles.Add(Bundle);
		Instance->bIssued = true;
		IssuedItemIds.Add(ItemId);
		IssuedThisTick.Add(ItemId);
	}

	for (const FGuid& ItemId : IssuedThisTick)
	{
		SpatialOrder.Remove(ItemId);
		PendingSpatialData.Remove(ItemId);
		ResolvedInstances.Remove(ItemId);
	}
}

void UVRegionResolver::UpdateOrCreateInstance(const FVMSpatialItemNet& Item)
{
	const FGuid ItemId = Item.ItemID.Value;
	if (!ItemId.IsValid())
	{
		return;
	}

	FResolvedSpatialInstance& Instance = ResolvedInstances.FindOrAdd(ItemId);
	Instance.SpatialId = ItemId;
	Instance.PayloadType = Item.PayloadType;
	Instance.CollisionId = Item.CollisionID.Value;
	Instance.SpatialItem = Item;
	Instance.MaterialIds.Reset();

	if (Item.PayloadType == ESpatialItemType::Mesh)
	{
		for (const FVMGuidNet& MatNet : Item.MeshPayload.material_ids)
		{
			if (MatNet.Value.IsValid())
			{
				Instance.MaterialIds.Add(MatNet.Value);
			}
		}
		const FGuid MeshId = Item.MeshPayload.mesh_ref.id.Value;
		if (MeshId.IsValid() && !RequestedMeshIds.Contains(MeshId))
		{
			if (!AssetManager.IsValid() || !AssetManager->IsMemoryCached(MeshId, FVAssetType::Mesh))
			{
				if (AssetManager.IsValid())
				{
					RequestedMeshIds.Add(MeshId);
					AssetManager->RequestStaticMeshAsync(Item.MeshPayload, FVOnStaticMeshLoaded());
				}
			}
		}
	}
	else if (Item.PayloadType == ESpatialItemType::Decal)
	{
		if (Item.DecalPayload.material_id.Value.IsValid())
		{
			Instance.MaterialIds.Add(Item.DecalPayload.material_id.Value);
		}
	}

	UpdateTextureDependencies(Instance);
}

void UVRegionResolver::UpdateTextureDependencies(FResolvedSpatialInstance& Instance)
{
	Instance.TextureIds.Reset();
	if (!AssetManager.IsValid() || !AssetManager->MaterialAgent)
	{
		return;
	}

	for (const FGuid& MatId : Instance.MaterialIds)
	{
		FVMMaterial MaterialItem;
		if (!AssetManager->MaterialAgent->GetMaterialItem(MatId, MaterialItem))
		{
			const FVMMaterial* Pending = PendingMaterials.Find(MatId);
			if (Pending)
			{
				MaterialItem = *Pending;
			}
		}

		for (const FVMTexture& TextureItem : MaterialItem.textures)
		{
			const FGuid TextureId = TextureItem.asset_ref.id;
			if (TextureId.IsValid())
			{
				Instance.TextureIds.Add(TextureId);
				RequestedTextureIds.Add(TextureId);
			}
		}
	}
}

void UVRegionResolver::IssueDependencyRequests()
{
	if (!RegionBridge.IsValid())
	{
		return;
	}

	if (!bMaterialRequestInFlight)
	{
		TArray<FGuid> MaterialBatch;
		for (TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
		{
			FResolvedSpatialInstance& Instance = Pair.Value;
			for (const FGuid& MatId : Instance.MaterialIds)
			{
				if (!MatId.IsValid() || RequestedMaterialIds.Contains(MatId))
				{
					continue;
				}
				if (AssetManager.IsValid() && AssetManager->IsMemoryCached(MatId, FVAssetType::Material))
				{
					continue;
				}
				Instance.bRequestedMaterials = true;
				RequestedMaterialIds.Add(MatId);
				MaterialBatch.Add(MatId);
				if (MaterialBatch.Num() >= MaxMaterialItemLimit)
				{
					break;
				}
			}
			if (MaterialBatch.Num() >= MaxMaterialItemLimit)
			{
				break;
			}
		}

		if (MaterialBatch.Num() > 0)
		{
			bMaterialRequestInFlight = true;
			RegionBridge->RequestMaterialsBatch(MaterialBatch);
		}
	}

	if (!bCollisionRequestInFlight)
	{
		TArray<FGuid> CollisionBatch;
		for (TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
		{
			FResolvedSpatialInstance& Instance = Pair.Value;
			const FGuid ColId = Instance.CollisionId;
			if (!ColId.IsValid() || RequestedCollisionIds.Contains(ColId))
			{
				continue;
			}
			if (PendingCollisions.Contains(ColId))
			{
				continue;
			}
			Instance.bRequestedCollision = true;
			RequestedCollisionIds.Add(ColId);
			CollisionBatch.Add(ColId);
			if (CollisionBatch.Num() >= MaxCollisionItemLimit)
			{
				break;
			}
		}

		if (CollisionBatch.Num() > 0)
		{
			bCollisionRequestInFlight = true;
			RegionBridge->RequestCollisionsBatch(CollisionBatch);
		}
	}
}

bool UVRegionResolver::AreMaterialsReady(const FResolvedSpatialInstance& Instance) const
{
	if (!AssetManager.IsValid())
	{
		return false;
	}

	for (const FGuid& MatId : Instance.MaterialIds)
	{
		if (!MatId.IsValid())
		{
			continue;
		}
		if (!AssetManager->IsMemoryCached(MatId, FVAssetType::Material))
		{
			return false;
		}
	}
	return true;
}

bool UVRegionResolver::AreTexturesReady(const FResolvedSpatialInstance& Instance) const
{
	if (!AssetManager.IsValid())
	{
		return false;
	}

	for (const FGuid& TexId : Instance.TextureIds)
	{
		if (!TexId.IsValid())
		{
			continue;
		}
		if (!AssetManager->IsMemoryCached(TexId, FVAssetType::Texture))
		{
			return false;
		}
	}
	return true;
}

bool UVRegionResolver::IsMeshReady(const FResolvedSpatialInstance& Instance) const
{
	if (!AssetManager.IsValid())
	{
		return false;
	}

	const FGuid MeshId = Instance.SpatialItem.MeshPayload.mesh_ref.id.Value;
	if (!MeshId.IsValid())
	{
		return false;
	}

	return AssetManager->IsMemoryCached(MeshId, FVAssetType::Mesh);
}

bool UVRegionResolver::IsCollisionReady(const FResolvedSpatialInstance& Instance) const
{
	if (!Instance.CollisionId.IsValid())
	{
		return true;
	}
	return PendingCollisions.Contains(Instance.CollisionId);
}

bool UVRegionResolver::CanRender(const FResolvedSpatialInstance& Instance) const
{
	if (Instance.PayloadType == ESpatialItemType::Mesh)
	{
		return IsMeshReady(Instance) && AreMaterialsReady(Instance) && AreTexturesReady(Instance);
	}
	if (Instance.PayloadType == ESpatialItemType::Decal)
	{
		return AreMaterialsReady(Instance) && AreTexturesReady(Instance);
	}
	return true;
}

bool UVRegionResolver::CanActivate(const FResolvedSpatialInstance& Instance) const
{
	if (Instance.CollisionId.IsValid())
	{
		return IsCollisionReady(Instance);
	}
	return true;
}
