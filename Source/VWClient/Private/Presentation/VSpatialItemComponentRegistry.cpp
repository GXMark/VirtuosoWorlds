#include "Presentation/VSpatialItemComponentRegistry.h"

#include "Components/SceneComponent.h"

void UVSpatialItemComponentRegistry::Initialize(AActor* InOwner, USceneComponent* InPresentationRoot)
{
	OwnerActor = InOwner;
	PresentationRoot = InPresentationRoot;
}

USceneComponent* UVSpatialItemComponentRegistry::GetOrCreateItemRoot(const FGuid& ItemId)
{
	if (!OwnerActor || !PresentationRoot.IsValid())
	{
		return nullptr;
	}

	if (TObjectPtr<USceneComponent>* Existing = ItemRootsById.Find(ItemId))
	{
		return Existing->Get();
	}

	const FString Name = FString::Printf(TEXT("VW_ItemRoot_%s"), *ItemId.ToString());
	USceneComponent* ItemRoot = NewObject<USceneComponent>(OwnerActor.Get(), *Name);
	ItemRoot->SetupAttachment(PresentationRoot.Get());
	ItemRoot->RegisterComponent();
	ItemRootsById.Add(ItemId, ItemRoot);
	return ItemRoot;
}

USceneComponent* UVSpatialItemComponentRegistry::FindItemRoot(const FGuid& ItemId) const
{
	if (const TObjectPtr<USceneComponent>* Existing = ItemRootsById.Find(ItemId))
	{
		return Existing->Get();
	}
	return nullptr;
}

void UVSpatialItemComponentRegistry::DestroyItemRoot(const FGuid& ItemId)
{
	if (TObjectPtr<USceneComponent>* Existing = ItemRootsById.Find(ItemId))
	{
		if (USceneComponent* ItemRoot = Existing->Get())
		{
			ItemRoot->DestroyComponent();
		}
		ItemRootsById.Remove(ItemId);
	}
}

void UVSpatialItemComponentRegistry::DestroyAllItemRoots()
{
	for (TPair<FGuid, TObjectPtr<USceneComponent>>& Pair : ItemRootsById)
	{
		if (USceneComponent* ItemRoot = Pair.Value.Get())
		{
			ItemRoot->DestroyComponent();
		}
	}
	ItemRootsById.Reset();
}
