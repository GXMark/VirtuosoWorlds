#include "Presentation/VItemPresenter.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Presentation/VMTransformUtils.h"

void UVItemPresenter::Initialize(
	AActor* InOwner,
	USceneComponent* InPresentationRoot,
	UVAssetManager* InAssetManager)
{
	PresentationOwner = InOwner;
	PresentationRoot = InPresentationRoot;
	AssetManager = InAssetManager;
}


void UVItemPresenter::PresentSpatialItemsBatch(const TArray<FVMSpatialItemNet>& Items)
{
	for (const FVMSpatialItemNet& Item : Items)
	{
		// For now we treat all incoming items as "add or update"
		OnItemUpdated(Item);
	}
}



void UVItemPresenter::OnItemAdded(const FVMSpatialItemNet& InItem)
{
	const FGuid ItemGuid = InItem.ItemID.Value;
	const FGuid ParentGuid = InItem.ParentID.Value;

	if (InItem.PayloadType != ESpatialItemType::Mesh)
		return;

	SpawnOrUpdateMesh(ItemGuid, InItem.MeshPayload, VMToTransform(InItem.Transform), ParentGuid);
}

void UVItemPresenter::OnItemUpdated(const FVMSpatialItemNet& InItem)
{
	const FGuid ItemGuid = InItem.ItemID.Value;
	const FGuid ParentGuid = InItem.ParentID.Value;
	
	if (InItem.PayloadType != ESpatialItemType::Mesh)
		return;

	SpawnOrUpdateMesh(ItemGuid, InItem.MeshPayload, VMToTransform(InItem.Transform), ParentGuid);
}

void UVItemPresenter::OnItemRemoved(const FGuid& InItemId)
{
	DestroyItem(InItemId);
}

void UVItemPresenter::SpawnOrUpdateMesh(
	const FGuid& InItemId,
	const FVMMeshComponentNet& InMeshData,
	const FTransform& InWorldTransform,
	const FGuid& InParentId)
{
	if (!PresentationOwner.IsValid() || !PresentationRoot.IsValid() || !AssetManager)
	{
		return;
	}
	
	// --------------------------------------------------
	// Log which Item requested which Mesh
	// --------------------------------------------------
	const FGuid& MeshId = InMeshData.mesh_ref.id.Value;
	UE_LOG(LogTemp, Log,
		TEXT("ItemPresenter: ItemId=%s ParentId=%s Requested MeshId=%s"),
		*InItemId.ToString(),
		InParentId.IsValid() ? *InParentId.ToString() : TEXT("<None>"),
		MeshId.IsValid() ? *MeshId.ToString() : TEXT("<None>") );

	USceneComponent* ExistingComp = SpawnedComponents.FindRef(InItemId);

	if (!ExistingComp)
	{
		UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(PresentationOwner.Get());
		MeshComp->AttachToComponent(PresentationRoot.Get(), FAttachmentTransformRules::KeepWorldTransform);
		MeshComp->RegisterComponent();

		SpawnedComponents.Add(InItemId, MeshComp);
		ExistingComp = MeshComp;
	}

	ExistingComp->SetWorldTransform(InWorldTransform);

	if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(ExistingComp))
	{
		ApplyMaterialsAsync(InItemId, MeshComp, InMeshData.material_ids);
	}

	const FGuid DesiredMeshId = InMeshData.mesh_ref.id.Value;

	// If we already requested (or set) this mesh for this item, do nothing.
	if (const FGuid* CurrentDesired = RequestedMeshByItemId.Find(InItemId))
	{
		if (*CurrentDesired == DesiredMeshId)
		{
			return;
		}
	}

	RequestedMeshByItemId.Add(InItemId, DesiredMeshId);

	FVOnStaticMeshLoaded Callback = FVOnStaticMeshLoaded::CreateLambda(
		[this, InItemId, DesiredMeshId](UStaticMesh* LoadedMesh)
		{
			if (!LoadedMesh)
			{
				UE_LOG(LogTemp, Warning,
				       TEXT("ItemPresenter: Mesh load FAILED (null). ItemId=%s DesiredMeshId=%s"),
				       *InItemId.ToString(), *DesiredMeshId.ToString());

				return;
			}

			const FGuid* CurrentDesired = RequestedMeshByItemId.Find(InItemId);
			if (!CurrentDesired || *CurrentDesired != DesiredMeshId)
			{
				return;
			}

			if (USceneComponent* Comp = SpawnedComponents.FindRef(InItemId))
			{
				if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comp))
				{
					MeshComp->SetStaticMesh(LoadedMesh);
				}
			}
		});

	AssetManager->RequestStaticMeshAsync(InMeshData, Callback);
}

void UVItemPresenter::ApplyMaterialsAsync(
	const FGuid& InItemId,
	UStaticMeshComponent* MeshComp,
	const TArray<FVMGuidNet>& InMaterialIds)
{
	if (!MeshComp || !AssetManager)
	{
		return;
	}

	TArray<FGuid> Desired;
	Desired.Reserve(InMaterialIds.Num());
	for (const FVMGuidNet& NetId : InMaterialIds)
	{
		Desired.Add(NetId.Value);
	}

	// Compare with last requested material set for this item.
	if (const TArray<FGuid>* Prev = RequestedMaterialsByItemId.Find(InItemId))
	{
		if (Prev->Num() == Desired.Num())
		{
			bool bSame = true;
			for (int32 i = 0; i < Desired.Num(); ++i)
			{
				if ((*Prev)[i] != Desired[i])
				{
					bSame = false;
					break;
				}
			}
			if (bSame)
			{
				return;
			}
		}
	}

	RequestedMaterialsByItemId.Add(InItemId, Desired);

	for (int32 SlotIndex = 0; SlotIndex < Desired.Num(); ++SlotIndex)
	{
		const FGuid MatId = Desired[SlotIndex];
		if (!MatId.IsValid())
		{
			continue;
		}

		AssetManager->RequestMaterialInstanceAsync(
			MatId,
			FVOnMaterialInstanceLoaded::CreateLambda(
				[this, InItemId, MatId, SlotIndex](UMaterialInstanceDynamic* LoadedMID)
				{
					if (!LoadedMID)
					{
						return;
					}

					const TArray<FGuid>* CurrentDesired = RequestedMaterialsByItemId.Find(InItemId);
					if (!CurrentDesired || !CurrentDesired->IsValidIndex(SlotIndex) || (*CurrentDesired)[SlotIndex] != MatId)
					{
						return;
					}

					if (USceneComponent* Comp = SpawnedComponents.FindRef(InItemId))
					{
						if (UStaticMeshComponent* MeshCompLocal = Cast<UStaticMeshComponent>(Comp))
						{
							MeshCompLocal->SetMaterial(SlotIndex, LoadedMID);
						}
					}
				}))
			;
	}
}

void UVItemPresenter::DestroyItem(const FGuid& InItemId)
{
	if (USceneComponent* Comp = SpawnedComponents.FindRef(InItemId))
	{
		Comp->DestroyComponent();
	}

	SpawnedComponents.Remove(InItemId);
	RequestedMeshByItemId.Remove(InItemId);
	RequestedMaterialsByItemId.Remove(InItemId);
}
