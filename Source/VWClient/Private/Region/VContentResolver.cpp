#include "Region/VContentResolver.h"

#include "Presentation/VCollisionPresenter.h"
#include "Presentation/VDecalPresenter.h"
#include "Presentation/VLightPresenter.h"
#include "Presentation/VMaterialPresenter.h"
#include "Presentation/VMeshPresenter.h"
#include "Region/VRegionClientBridge.h"
#include "Subsystem/VAssetManager.h"
#include "Model/Constant/VConstants.h"
#include "Materials/MaterialInterface.h"

void UVContentResolver::Initialize(
	AActor* InOwner,
	URegionClientBridge* InRegionBridge,
	UVMeshPresenter* InMeshPresenter,
	UVMaterialPresenter* InMaterialPresenter,
	UVLightPresenter* InLightPresenter,
	UVDecalPresenter* InDecalPresenter,
	UVCollisionPresenter* InCollisionPresenter,
	UVAssetManager* InAssetManager)
{
	OwnerActor = InOwner;
	RegionBridge = InRegionBridge;
	MeshPresenter = InMeshPresenter;
	MaterialPresenter = InMaterialPresenter;
	LightPresenter = InLightPresenter;
	DecalPresenter = InDecalPresenter;
	CollisionPresenter = InCollisionPresenter;
	AssetManager = InAssetManager;

	if (MeshPresenter.IsValid())
	{
		MeshPresenter->SetOnMeshComponentReady(
			UVMeshPresenter::FOnMeshComponentReady::CreateUObject(
				this, &UVContentResolver::RegisterMeshComponent));
	}

	if (CollisionPresenter.IsValid())
	{
		CollisionPresenter->SetOnCollisionRootReady(
			UVCollisionPresenter::FOnCollisionRootReady::CreateUObject(
				this, &UVContentResolver::RegisterCollisionComponent));
	}
}

void UVContentResolver::Tick(float DeltaSeconds)
{
	(void)DeltaSeconds;
	if (!OwnerActor.IsValid())
	{
		return;
	}

	for (const TPair<FGuid, FVMSpatialItemNet>& Pair : PendingSpatialData)
	{
		UpdateOrCreateInstance(Pair.Value);
	}

	for (TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
	{
		EvaluateInstance(Pair.Value);
	}

	IssueDependencyRequests();
}

void UVContentResolver::OnSpatialBatchReceived(const TArray<FVMSpatialItemNet>& Items)
{
	for (const FVMSpatialItemNet& Item : Items)
	{
		const FGuid ItemId = Item.ItemID.Value;
		if (!ItemId.IsValid())
		{
			continue;
		}
		PendingSpatialData.Add(ItemId, Item);
		UpdateOrCreateInstance(Item);
	}
}

void UVContentResolver::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
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

void UVContentResolver::OnCollisionsBatchReceived(const TArray<FVMCollision>& Collisions)
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

	if (CollisionPresenter.IsValid())
	{
		CollisionPresenter->SubmitCollisionDefs(Collisions);
	}

	bCollisionRequestInFlight = false;
}

void UVContentResolver::OnSpatialItemRemoved(const FGuid& ItemId)
{
	PendingSpatialData.Remove(ItemId);

	if (FResolvedSpatialInstance* Instance = ResolvedInstances.Find(ItemId))
	{
		if (Instance->ResolutionState != EResolutionState::Retired)
		{
			LogStateTransition(*Instance, EResolutionState::Retired);
		}
		Instance->ResolutionState = EResolutionState::Retired;
	}

	ReleaseSpatial(ItemId);
	ResolvedInstances.Remove(ItemId);
}

void UVContentResolver::DumpUnresolvedSpatialItems() const
{
	int32 Count = 0;
	for (const TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
	{
		if (Pair.Value.ResolutionState != EResolutionState::Active)
		{
			UE_LOG(LogTemp, Log,
				TEXT("ContentResolver: Unresolved SpatialId=%s State=%d"),
				*Pair.Key.ToString(), static_cast<int32>(Pair.Value.ResolutionState));
			++Count;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("ContentResolver: Unresolved count=%d"), Count);
}

void UVContentResolver::DumpActiveSpatialCount() const
{
	int32 Count = 0;
	for (const TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
	{
		if (Pair.Value.ResolutionState == EResolutionState::Active)
		{
			++Count;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("ContentResolver: Active spatial count=%d"), Count);
}

void UVContentResolver::UpdateOrCreateInstance(const FVMSpatialItemNet& Item)
{
	const FGuid ItemId = Item.ItemID.Value;
	if (!ItemId.IsValid())
	{
		return;
	}

	FResolvedSpatialInstance& Instance = ResolvedInstances.FindOrAdd(ItemId);
	if (Instance.SpatialId != ItemId)
	{
		Instance.SpatialId = ItemId;
	}
	Instance.PayloadType = Item.PayloadType;
	Instance.CollisionId = Item.CollisionID.Value;
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
	}
	else if (Item.PayloadType == ESpatialItemType::Decal)
	{
		if (Item.DecalPayload.material_id.Value.IsValid())
		{
			Instance.MaterialIds.Add(Item.DecalPayload.material_id.Value);
		}
	}

	UpdateTextureDependencies(Instance);

	if (Instance.ResolutionState == EResolutionState::Declared)
	{
		LogStateTransition(Instance, EResolutionState::Resolving);
		Instance.ResolutionState = EResolutionState::Resolving;
	}
}

void UVContentResolver::EvaluateInstance(FResolvedSpatialInstance& Instance)
{
	const FVMSpatialItemNet* Item = PendingSpatialData.Find(Instance.SpatialId);
	if (!Item)
	{
		if (Instance.ResolutionState == EResolutionState::Active)
		{
			return;
		}
		if (Instance.ResolutionState != EResolutionState::Retired)
		{
			LogStateTransition(Instance, EResolutionState::Retired);
			Instance.ResolutionState = EResolutionState::Retired;
		}
		return;
	}

	if (Instance.ResolutionState == EResolutionState::Retired)
	{
		return;
	}

	if (!Instance.bMeshPresented && Item->PayloadType == ESpatialItemType::Mesh)
	{
		PresentSpatial(Instance, *Item);
	}

	if (Item->PayloadType == ESpatialItemType::Mesh)
	{
		if (!Instance.bMaterialPresented && CanRender(Instance))
		{
			PresentSpatial(Instance, *Item);
		}
	}
	else if (Item->PayloadType == ESpatialItemType::Decal || Item->PayloadType == ESpatialItemType::PointLight
		|| Item->PayloadType == ESpatialItemType::SpotLight)
	{
		if (!Instance.bDecalOrLightPresented && CanRender(Instance))
		{
			PresentSpatial(Instance, *Item);
		}
	}

	if (!Instance.bCollisionPresented && CanActivate(Instance))
	{
		PresentSpatial(Instance, *Item);
	}

	if (Instance.ResolutionState == EResolutionState::Resolving && CanRender(Instance))
	{
		LogStateTransition(Instance, EResolutionState::RenderableReady);
		Instance.ResolutionState = EResolutionState::RenderableReady;
	}

	if (Instance.ResolutionState != EResolutionState::Active && CanActivate(Instance) && CanRender(Instance))
	{
		LogStateTransition(Instance, EResolutionState::Active);
		Instance.ResolutionState = EResolutionState::Active;
		PendingSpatialData.Remove(Instance.SpatialId);
		PrunePayloadsForInstance(Instance);
	}
}

void UVContentResolver::IssueDependencyRequests()
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

void UVContentResolver::PresentSpatial(FResolvedSpatialInstance& Instance, const FVMSpatialItemNet& Item)
{
	const FTransform ItemXf(
		Item.Transform.Rotation.ToQuat(),
		FVector(Item.Transform.Location),
		Item.Transform.bHasScale ? FVector(Item.Transform.Scale) : FVector(1.f, 1.f, 1.f));

	switch (Item.PayloadType)
	{
	case ESpatialItemType::Mesh:
	{
		if (!Instance.bMeshPresented && MeshPresenter.IsValid())
		{
			UStaticMeshComponent* MeshComp = MeshPresenter->PresentMeshItem(
				Item.ItemID.Value,
				Item.MeshPayload,
				ItemXf,
				Item.ParentID.Value);
			RegisterMeshComponent(Item.ItemID.Value, MeshComp);
			Instance.bMeshPresented = true;
		}

		if (!Instance.bMaterialPresented && MaterialPresenter.IsValid() && CanRender(Instance))
		{
			TArray<UMaterialInterface*> Materials;
			Materials.Reserve(Instance.MaterialIds.Num());
			if (AssetManager.IsValid() && AssetManager->MaterialAgent)
			{
				for (const FGuid& MatId : Instance.MaterialIds)
				{
					Materials.Add(AssetManager->MaterialAgent->GetMaterial(MatId));
				}
			}

			if (Instance.MeshComponent.IsValid())
			{
				MaterialPresenter->ApplyMaterials(Item.ItemID.Value, Instance.MeshComponent.Get(), Materials);
				Instance.bMaterialPresented = true;
			}
		}
		break;
	}
	case ESpatialItemType::PointLight:
		if (!Instance.bDecalOrLightPresented && LightPresenter.IsValid())
		{
			LightPresenter->PresentPointLightItem(Item.ItemID.Value, Item.PointLightPayload, ItemXf);
			Instance.bDecalOrLightPresented = true;
			Instance.LightComponent = LightPresenter->FindLightComponent(Item.ItemID.Value);
		}
		break;
	case ESpatialItemType::SpotLight:
		if (!Instance.bDecalOrLightPresented && LightPresenter.IsValid())
		{
			LightPresenter->PresentSpotLightItem(Item.ItemID.Value, Item.SpotLightPayload, ItemXf);
			Instance.bDecalOrLightPresented = true;
			Instance.LightComponent = LightPresenter->FindLightComponent(Item.ItemID.Value);
		}
		break;
	case ESpatialItemType::Decal:
		if (!Instance.bDecalOrLightPresented && DecalPresenter.IsValid() && CanRender(Instance))
		{
			UMaterialInterface* DecalMaterial = nullptr;
			if (AssetManager.IsValid() && AssetManager->MaterialAgent && Instance.MaterialIds.Num() > 0)
			{
				DecalMaterial = AssetManager->MaterialAgent->GetMaterial(Instance.MaterialIds[0]);
			}

			DecalPresenter->PresentDecalItem(Item.ItemID.Value, Item.DecalPayload, ItemXf, DecalMaterial);
			Instance.bDecalOrLightPresented = true;
			Instance.DecalComponent = DecalPresenter->FindDecalComponent(Item.ItemID.Value);
		}
		break;
	default:
		break;
	}

	if (!Instance.bCollisionPresented && CanActivate(Instance) && CollisionPresenter.IsValid())
	{
		CollisionPresenter->PresentCollision(Item.ItemID.Value, ItemXf, Item.CollisionID.Value);
		Instance.bCollisionPresented = true;
		Instance.CollisionRoot = CollisionPresenter->FindCollisionRoot(Item.ItemID.Value);
	}
}

void UVContentResolver::ReleaseSpatial(const FGuid& ItemId)
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
	if (MaterialPresenter.IsValid())
	{
		MaterialPresenter->ForgetItem(ItemId);
	}
}

void UVContentResolver::PrunePayloadsForInstance(const FResolvedSpatialInstance& Instance)
{
	for (const FGuid& MatId : Instance.MaterialIds)
	{
		bool bUsedElsewhere = false;
		for (const TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
		{
			if (Pair.Value.SpatialId == Instance.SpatialId)
			{
				continue;
			}
			if (Pair.Value.MaterialIds.Contains(MatId))
			{
				bUsedElsewhere = true;
				break;
			}
		}
		if (!bUsedElsewhere)
		{
			PendingMaterials.Remove(MatId);
		}
	}

	if (Instance.CollisionId.IsValid())
	{
		bool bUsedElsewhere = false;
		for (const TPair<FGuid, FResolvedSpatialInstance>& Pair : ResolvedInstances)
		{
			if (Pair.Value.SpatialId == Instance.SpatialId)
			{
				continue;
			}
			if (Pair.Value.CollisionId == Instance.CollisionId)
			{
				bUsedElsewhere = true;
				break;
			}
		}
		if (!bUsedElsewhere)
		{
			PendingCollisions.Remove(Instance.CollisionId);
		}
	}
}

void UVContentResolver::RegisterMeshComponent(const FGuid& ItemId, UStaticMeshComponent* MeshComponent)
{
	if (!MeshComponent)
	{
		return;
	}
	if (FResolvedSpatialInstance* Instance = ResolvedInstances.Find(ItemId))
	{
		Instance->MeshComponent = MeshComponent;
	}
}

void UVContentResolver::RegisterCollisionComponent(const FGuid& ItemId, USceneComponent* CollisionRoot)
{
	if (!CollisionRoot)
	{
		return;
	}
	if (FResolvedSpatialInstance* Instance = ResolvedInstances.Find(ItemId))
	{
		Instance->CollisionRoot = CollisionRoot;
	}
}

void UVContentResolver::UpdateTextureDependencies(FResolvedSpatialInstance& Instance)
{
	Instance.TextureIds.Reset();
	Instance.bRequestedTextures = false;
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
	Instance.bRequestedTextures = Instance.TextureIds.Num() > 0;
}

bool UVContentResolver::AreMaterialsReady(const FResolvedSpatialInstance& Instance) const
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

bool UVContentResolver::AreTexturesReady(const FResolvedSpatialInstance& Instance) const
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

bool UVContentResolver::IsCollisionReady(const FResolvedSpatialInstance& Instance) const
{
	if (!Instance.CollisionId.IsValid())
	{
		return true;
	}
	return PendingCollisions.Contains(Instance.CollisionId);
}

bool UVContentResolver::CanRender(const FResolvedSpatialInstance& Instance) const
{
	if (Instance.PayloadType == ESpatialItemType::Mesh || Instance.PayloadType == ESpatialItemType::Decal)
	{
		return AreMaterialsReady(Instance) && AreTexturesReady(Instance);
	}
	return true;
}

bool UVContentResolver::CanActivate(const FResolvedSpatialInstance& Instance) const
{
	if (Instance.CollisionId.IsValid())
	{
		return IsCollisionReady(Instance);
	}
	return true;
}

void UVContentResolver::LogStateTransition(const FResolvedSpatialInstance& Instance, EResolutionState NewState) const
{
	UE_LOG(LogTemp, Log,
		TEXT("ContentResolver: SpatialId=%s State %d -> %d"),
		*Instance.SpatialId.ToString(),
		static_cast<int32>(Instance.ResolutionState),
		static_cast<int32>(NewState));
}
