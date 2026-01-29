#include "Presentation/VMeshPresenter.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Subsystem/VAssetManager.h"

void UVMeshPresenter::Initialize(AActor* InOwner, USceneComponent* InPresentationRoot, UVAssetManager* InAssetManager)
{
	PresentationOwner = InOwner;
	PresentationRoot = InPresentationRoot;
	AssetManager = InAssetManager;
}

UStaticMeshComponent* UVMeshPresenter::PresentMeshItem(
	const FGuid& InItemId,
	const FVMMeshComponentNet& InMeshData,
	const FTransform& InWorldTransform,
	const FGuid& InParentId)
{
	if (!PresentationOwner.IsValid() || !PresentationRoot.IsValid() || !AssetManager)
	{
		return nullptr;
	}

	const FGuid& MeshId = InMeshData.mesh_ref.id.Value;
	UE_LOG(LogTemp, Log,
		TEXT("MeshPresenter: ItemId=%s ParentId=%s Requested MeshId=%s"),
		*InItemId.ToString(),
		InParentId.IsValid() ? *InParentId.ToString() : TEXT("<None>"),
		MeshId.IsValid() ? *MeshId.ToString() : TEXT("<None>") );

	UStaticMeshComponent* ExistingComp = SpawnedComponents.FindRef(InItemId);
	if (!ExistingComp)
	{
		ExistingComp = NewObject<UStaticMeshComponent>(PresentationOwner.Get());
		ExistingComp->AttachToComponent(PresentationRoot.Get(), FAttachmentTransformRules::KeepWorldTransform);
		ExistingComp->RegisterComponent();
		SpawnedComponents.Add(InItemId, ExistingComp);
	}

	ExistingComp->SetWorldTransform(InWorldTransform);

	const FGuid DesiredMeshId = InMeshData.mesh_ref.id.Value;
	if (const FGuid* CurrentDesired = RequestedMeshByItemId.Find(InItemId))
	{
		if (*CurrentDesired == DesiredMeshId)
		{
			return ExistingComp;
		}
	}

	RequestedMeshByItemId.Add(InItemId, DesiredMeshId);

	FVOnStaticMeshLoaded Callback = FVOnStaticMeshLoaded::CreateLambda(
		[this, InItemId, DesiredMeshId](UStaticMesh* LoadedMesh)
		{
			if (!LoadedMesh)
			{
				UE_LOG(LogTemp, Warning,
					TEXT("MeshPresenter: Mesh load FAILED (null). ItemId=%s DesiredMeshId=%s"),
					*InItemId.ToString(), *DesiredMeshId.ToString());
				return;
			}

			const FGuid* CurrentDesired = RequestedMeshByItemId.Find(InItemId);
			if (!CurrentDesired || *CurrentDesired != DesiredMeshId)
			{
				return;
			}

			if (UStaticMeshComponent* MeshComp = SpawnedComponents.FindRef(InItemId))
			{
				MeshComp->SetStaticMesh(LoadedMesh);
			}
		});

	AssetManager->RequestStaticMeshAsync(InMeshData, Callback);

	return ExistingComp;
}

UStaticMeshComponent* UVMeshPresenter::FindMeshComponent(const FGuid& InItemId) const
{
	return SpawnedComponents.FindRef(InItemId);
}

void UVMeshPresenter::DestroyItem(const FGuid& InItemId)
{
	if (UStaticMeshComponent* Comp = SpawnedComponents.FindRef(InItemId))
	{
		Comp->DestroyComponent();
	}

	SpawnedComponents.Remove(InItemId);
	RequestedMeshByItemId.Remove(InItemId);
}
