#include "Presentation/VCollisionPresenter.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"

void UVCollisionPresenter::Initialize(AActor* InOwner, USceneComponent* InCollisionRoot)
{
	OwnerActor = InOwner;
	CollisionRoot = InCollisionRoot;
}

void UVCollisionPresenter::OnItemUpsert(const FGuid& ItemId, const FTransform& WorldTransform, const FGuid& CollisionId)
{
	if (!OwnerActor || !CollisionRoot)
	{
		return;
	}

	ItemWorldTransformById.Add(ItemId, WorldTransform);
	CollisionIdByItemId.Add(ItemId, CollisionId);

	RebuildItem(ItemId);
}

void UVCollisionPresenter::SubmitCollisionDefs(const TArray<FVMCollision>& Collisions)
{
	for (const FVMCollision& Col : Collisions)
	{
		if (!Col.id.IsValid())
		{
			continue;
		}
		CollisionDefsById.Add(Col.id, Col);
	}

	// Rebuild any item that references the newly submitted collision ids.
	for (const FVMCollision& Col : Collisions)
	{
		if (!Col.id.IsValid())
		{
			continue;
		}

		for (const TPair<FGuid, FGuid>& Pair : CollisionIdByItemId)
		{
			if (Pair.Value == Col.id)
			{
				RebuildItem(Pair.Key);
			}
		}
	}
}

void UVCollisionPresenter::RebuildItem(const FGuid& ItemId)
{
	const FTransform* Xf = ItemWorldTransformById.Find(ItemId);
	const FGuid* ColIdPtr = CollisionIdByItemId.Find(ItemId);
	if (!Xf || !ColIdPtr)
	{
		return;
	}

	const FGuid ColId = *ColIdPtr;

	FVCollisionInstance& Instance = InstancesByItemId.FindOrAdd(ItemId);
	USceneComponent* ItemRoot = EnsureItemRoot(Instance, ItemId);
	if (!ItemRoot)
	{
		return;
	}

	// Always keep the item root transform in sync (moves all primitives).
	ItemRoot->SetWorldTransform(*Xf);

	// No collision for this item => clear primitives.
	if (!ColId.IsValid())
	{
		DestroyPrimitives(Instance);
		Instance.CollisionId = FGuid(0,0,0,0);
		return;
	}

	const FVMCollision* Def = CollisionDefsById.Find(ColId);
	if (!Def)
	{
		// We don't have the shared definition yet; keep transform, rebuild later.
		Instance.CollisionId = ColId;
		return;
	}

	// Full rebuild approach: destroy and recreate all primitive components.
	DestroyPrimitives(Instance);
	Instance.CollisionId = ColId;

	// Boxes
	for (int32 i = 0; i < Def->boxes.Num(); ++i)
	{
		const FVMBoxCollision& B = Def->boxes[i];
		UBoxComponent* Box = CreateBox(ItemRoot, *FString::Printf(TEXT("VW_Box_%d"), i));
		if (!Box) continue;

		Box->SetRelativeLocation(B.Center);
		Box->SetRelativeRotation(B.Rotation);
		Box->SetBoxExtent(B.Extents, true);
		Instance.BoxComponents.Add(Box);
	}

	// Spheres
	for (int32 i = 0; i < Def->spheres.Num(); ++i)
	{
		const FVMSphereCollision& S = Def->spheres[i];
		USphereComponent* Sphere = CreateSphere(ItemRoot, *FString::Printf(TEXT("VW_Sphere_%d"), i));
		if (!Sphere) continue;

		Sphere->SetRelativeLocation(S.center);
		Sphere->SetSphereRadius(S.radius, true);
		Instance.SphereComponents.Add(Sphere);
	}

	// Capsules
	for (int32 i = 0; i < Def->capsules.Num(); ++i)
	{
		const FVMCapsuleCollision& C = Def->capsules[i];
		UCapsuleComponent* Capsule = CreateCapsule(ItemRoot, *FString::Printf(TEXT("VW_Capsule_%d"), i));
		if (!Capsule) continue;

		Capsule->SetRelativeLocation(C.center);
		Capsule->SetRelativeRotation(C.rotation);
		// UE expects HalfHeight,Radius. We treat C.length as HalfHeight in your data model.
		Capsule->SetCapsuleSize(C.radius, C.length, true);
		Instance.CapsuleComponents.Add(Capsule);
	}

	// Convexes are intentionally ignored in this patch to avoid introducing ProceduralMeshComponent dependency.
}

void UVCollisionPresenter::DestroyPrimitives(FVCollisionInstance& Instance)
{
	for (UBoxComponent* C : Instance.BoxComponents)
	{
		if (C) C->DestroyComponent();
	}
	for (USphereComponent* C : Instance.SphereComponents)
	{
		if (C) C->DestroyComponent();
	}
	for (UCapsuleComponent* C : Instance.CapsuleComponents)
	{
		if (C) C->DestroyComponent();
	}

	Instance.BoxComponents.Reset();
	Instance.SphereComponents.Reset();
	Instance.CapsuleComponents.Reset();
}

USceneComponent* UVCollisionPresenter::EnsureItemRoot(FVCollisionInstance& Instance, const FGuid& ItemId)
{
	if (!OwnerActor || !CollisionRoot)
	{
		return nullptr;
	}

	if (!Instance.ItemRoot)
	{
		Instance.ItemRoot = NewObject<USceneComponent>(OwnerActor.Get(), *FString::Printf(TEXT("VW_CollisionItemRoot_%s"), *ItemId.ToString()));
		Instance.ItemRoot->SetupAttachment(CollisionRoot);
		Instance.ItemRoot->RegisterComponent();
	}

	return Instance.ItemRoot;
}

UBoxComponent* UVCollisionPresenter::CreateBox(USceneComponent* Parent, const FName& Name)
{
	if (!OwnerActor || !Parent)
	{
		return nullptr;
	}

	UBoxComponent* Comp = NewObject<UBoxComponent>(OwnerActor.Get(), Name);
	Comp->SetupAttachment(Parent);
	Comp->RegisterComponent();
	ConfigureCollision(Comp);
	return Comp;
}

USphereComponent* UVCollisionPresenter::CreateSphere(USceneComponent* Parent, const FName& Name)
{
	if (!OwnerActor || !Parent)
	{
		return nullptr;
	}

	USphereComponent* Comp = NewObject<USphereComponent>(OwnerActor.Get(), Name);
	Comp->SetupAttachment(Parent);
	Comp->RegisterComponent();
	ConfigureCollision(Comp);
	return Comp;
}

UCapsuleComponent* UVCollisionPresenter::CreateCapsule(USceneComponent* Parent, const FName& Name)
{
	if (!OwnerActor || !Parent)
	{
		return nullptr;
	}

	UCapsuleComponent* Comp = NewObject<UCapsuleComponent>(OwnerActor.Get(), Name);
	Comp->SetupAttachment(Parent);
	Comp->RegisterComponent();
	ConfigureCollision(Comp);
	return Comp;
}

void UVCollisionPresenter::ConfigureCollision(UPrimitiveComponent* Prim)
{
	if (!Prim)
	{
		return;
	}

	Prim->SetMobility(EComponentMobility::Movable);
	Prim->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Prim->SetGenerateOverlapEvents(false);
	Prim->PrimaryComponentTick.bCanEverTick = false;
}
