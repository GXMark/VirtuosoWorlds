#include "Presentation/VChunkPresenter.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

static FORCEINLINE FTransform VMToTransform(const FVMTransformNet& Net)
{
	FTransform Xf;
	Xf.SetLocation(FVector(Net.Location));
	Xf.SetRotation(Net.Rotation.ToQuat());
	Xf.SetScale3D(Net.bHasScale ? FVector(Net.Scale) : FVector(1.f, 1.f, 1.f));
	return Xf;
}

void UVChunkPresenter::Initialize(
	AActor* InOwner,
	USceneComponent* InPresentationRoot,
	UVAssetManager* InAssetManager)
{
	PresentationOwner = InOwner;
	PresentationRoot = InPresentationRoot;
	AssetManager = InAssetManager;
}

void UVChunkPresenter::ObserveChunk(AVReplicatedChunk* InChunk)
{
	if (!InChunk)
	{
		return;
	}

	InChunk->OnItemAdded.AddUObject(this, &UVChunkPresenter::OnItemAdded);
	InChunk->OnItemUpdated.AddUObject(this, &UVChunkPresenter::OnItemUpdated);
	InChunk->OnItemRemoved.AddUObject(this, &UVChunkPresenter::OnItemRemoved);

	// === HYDRATE EXISTING STATE ===
	const TArray<FVReplicatedSpatialItem>& ReplicatedItems =
		InChunk->SpatialItems.Items;

	UE_LOG(LogTemp, Log,
		TEXT("ChunkPresenter: Observing chunk %s with %d existing items"),
		*GetNameSafe(InChunk),
		ReplicatedItems.Num()
	);

	for (const FVReplicatedSpatialItem& RepItem : ReplicatedItems)
	{
		// This matches your delegate semantics
		OnItemAdded(RepItem.Item);
	}
}


void UVChunkPresenter::OnItemAdded(const FVMSpatialItemNet& InItem)
{
	const FGuid ItemGuid = InItem.ItemID.Value;
	const FGuid ParentGuid = InItem.ParentID.Value;

	if (InItem.PayloadType != ESpatialItemType::Mesh)
		return;

	SpawnOrUpdateMesh(ItemGuid, InItem.MeshPayload, VMToTransform(InItem.Transform), ParentGuid);
}

void UVChunkPresenter::OnItemUpdated(const FVMSpatialItemNet& InItem)
{
	const FGuid ItemGuid = InItem.ItemID.Value;
	const FGuid ParentGuid = InItem.ParentID.Value;
	
	if (InItem.PayloadType != ESpatialItemType::Mesh)
		return;

	SpawnOrUpdateMesh(ItemGuid, InItem.MeshPayload, VMToTransform(InItem.Transform), ParentGuid);
}

void UVChunkPresenter::OnItemRemoved(const FGuid& InItemId)
{
	DestroyItem(InItemId);
}

void UVChunkPresenter::SpawnOrUpdateMesh(
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
		TEXT("ChunkPresenter: ItemId=%s ParentId=%s Requested MeshId=%s"),
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
				       TEXT("ChunkPresenter: Mesh load FAILED (null). ItemId=%s DesiredMeshId=%s"),
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

void UVChunkPresenter::DestroyItem(const FGuid& InItemId)
{
	if (USceneComponent* Comp = SpawnedComponents.FindRef(InItemId))
	{
		Comp->DestroyComponent();
	}

	SpawnedComponents.Remove(InItemId);
	RequestedMeshByItemId.Remove(InItemId);
}
