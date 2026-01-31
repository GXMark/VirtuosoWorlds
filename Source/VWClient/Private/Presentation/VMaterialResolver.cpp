#include "Presentation/VMaterialResolver.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Region/VRegionClientBridge.h"
#include "Subsystem/VAssetManager.h"

void UVMaterialResolver::Initialize(UVAssetManager* InAssetManager, URegionClientBridge* InRegionBridge)
{
	AssetManager = InAssetManager;
	RegionBridge = InRegionBridge;
}

void UVMaterialResolver::ResolveMaterial(uint32 MaterialId, FVOnMaterialResolved Callback)
{
	if (MaterialId == 0)
	{
		if (Callback.IsBound())
		{
			Callback.Execute(MaterialId, nullptr);
		}
		return;
	}

	if (TObjectPtr<UMaterialInterface>* Cached = MaterialCacheById.Find(MaterialId))
	{
		if (Callback.IsBound())
		{
			Callback.Execute(MaterialId, Cached->Get());
		}
		return;
	}

	PendingCallbacksById.FindOrAdd(MaterialId).Add(MoveTemp(Callback));

	if (PendingMaterialRequests.Contains(MaterialId))
	{
		return;
	}

	const FGuid MaterialGuid = ResolveMaterialGuid(MaterialId);
	if (!MaterialGuid.IsValid())
	{
		HandleMaterialResolved(nullptr, MaterialId);
		return;
	}

	PendingMaterialRequests.Add(MaterialId);

	if (RegionBridge)
	{
		TArray<FGuid> MaterialBatch;
		MaterialBatch.Add(MaterialGuid);
		RegionBridge->RequestMaterialsBatch(MaterialBatch);
	}

	if (!AssetManager)
	{
		HandleMaterialResolved(nullptr, MaterialId);
		return;
	}

	AssetManager->RequestMaterialInstanceAsync(
		MaterialGuid,
		FVOnMaterialInstanceLoaded::CreateUObject(
			this,
			&UVMaterialResolver::HandleMaterialResolved,
			MaterialId));
}

void UVMaterialResolver::OnMaterialsBatchReceived(const TArray<FVMMaterial>& Materials)
{
	if (AssetManager)
	{
		AssetManager->SubmitMaterialItems(Materials);
	}
}

void UVMaterialResolver::HandleMaterialResolved(UMaterialInstanceDynamic* Material, uint32 MaterialId)
{
	PendingMaterialRequests.Remove(MaterialId);

	if (MaterialId != 0 && Material)
	{
		MaterialCacheById.Add(MaterialId, Material);
	}

	if (TArray<FVOnMaterialResolved>* Callbacks = PendingCallbacksById.Find(MaterialId))
	{
		for (FVOnMaterialResolved& Callback : *Callbacks)
		{
			if (Callback.IsBound())
			{
				Callback.Execute(MaterialId, Material);
			}
		}
		PendingCallbacksById.Remove(MaterialId);
	}
}

FGuid UVMaterialResolver::ResolveMaterialGuid(uint32 MaterialId) const
{
	if (MaterialId == 0)
	{
		return FGuid();
	}

	return FGuid(static_cast<int32>(MaterialId), 0, 0, 0);
}
