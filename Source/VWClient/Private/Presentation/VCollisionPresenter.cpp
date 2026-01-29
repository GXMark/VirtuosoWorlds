#include "Presentation/VCollisionPresenter.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/CollisionProfile.h"
#include "ProceduralMeshComponent.h"

void UVCollisionPresenter::Initialize(AActor* InOwner, USceneComponent* InCollisionRoot)
{
	OwnerActor = InOwner;
	CollisionRoot = InCollisionRoot ? InCollisionRoot : (InOwner ? InOwner->GetRootComponent() : nullptr);
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
		if (!LoadedCollisionIds.Contains(Col.id))
		{
			LoadedCollisionIds.Add(Col.id);
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

void UVCollisionPresenter::OnItemRemoved(const FGuid& ItemId)
{
	FVCollisionInstance* Instance = InstancesByItemId.Find(ItemId);
	if (Instance)
	{
		DestroyPrimitives(*Instance);
		if (Instance->ItemRoot)
		{
			UE_LOG(LogTemp, Log, TEXT("CollisionPresenter: Removed collision root for ItemId=%s"), *ItemId.ToString());
			Instance->ItemRoot->DestroyComponent();
			Instance->ItemRoot = nullptr;
		}
	}

	InstancesByItemId.Remove(ItemId);
	CollisionIdByItemId.Remove(ItemId);
	ItemWorldTransformById.Remove(ItemId);
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
		if (HasAnyPrimitives(Instance))
		{
			UE_LOG(LogTemp, Log, TEXT("CollisionPresenter: Clearing collision for ItemId=%s"), *ItemId.ToString());
			DestroyPrimitives(Instance);
		}
		Instance.CollisionId = FGuid(0, 0, 0, 0);
		return;
	}

	const FVMCollision* Def = CollisionDefsById.Find(ColId);
	if (!Def)
	{
		// We don't have the shared definition yet; keep transform, rebuild later.
		Instance.CollisionId = ColId;
		return;
	}

	if (Instance.CollisionId == ColId && HasAnyPrimitives(Instance))
	{
		// Already built for this collision id; only update transform.
		return;
	}

	// Full rebuild approach: destroy and recreate all primitive components.
	DestroyPrimitives(Instance);
	Instance.CollisionId = ColId;

	UE_LOG(LogTemp, Log,
		TEXT("CollisionPresenter: Building collision for ItemId=%s CollisionId=%s (Boxes=%d Spheres=%d Capsules=%d Convexes=%d)"),
		*ItemId.ToString(),
		*ColId.ToString(),
		Def->boxes.Num(),
		Def->spheres.Num(),
		Def->capsules.Num(),
		Def->convexes.Num());

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

	// Convexes
	for (int32 i = 0; i < Def->convexes.Num(); ++i)
	{
		const FVMConvexCollision& C = Def->convexes[i];
		if (C.vertices.Num() < 4)
		{
			UE_LOG(LogTemp, Warning, TEXT("CollisionPresenter: Skipping convex with insufficient vertices for ItemId=%s"), *ItemId.ToString());
			continue;
		}

		UProceduralMeshComponent* Convex = CreateConvex(ItemRoot, *FString::Printf(TEXT("VW_Convex_%d"), i), C.vertices);
		if (!Convex)
		{
			continue;
		}
		Instance.ConvexComponents.Add(Convex);
	}
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
	for (UProceduralMeshComponent* C : Instance.ConvexComponents)
	{
		if (C) C->DestroyComponent();
	}

	Instance.BoxComponents.Reset();
	Instance.SphereComponents.Reset();
	Instance.CapsuleComponents.Reset();
	Instance.ConvexComponents.Reset();
}

USceneComponent* UVCollisionPresenter::EnsureItemRoot(FVCollisionInstance& Instance, const FGuid& ItemId)
{
	if (!OwnerActor)
	{
		return nullptr;
	}

	if (!CollisionRoot)
	{
		CollisionRoot = OwnerActor->GetRootComponent();
	}

	if (!Instance.ItemRoot)
	{
		Instance.ItemRoot = NewObject<USceneComponent>(OwnerActor.Get(), *FString::Printf(TEXT("VW_CollisionItemRoot_%s"), *ItemId.ToString()));
		Instance.ItemRoot->SetupAttachment(CollisionRoot ? CollisionRoot : OwnerActor->GetRootComponent());
		Instance.ItemRoot->RegisterComponent();
		UE_LOG(LogTemp, Log, TEXT("CollisionPresenter: Created collision root for ItemId=%s"), *ItemId.ToString());
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

UProceduralMeshComponent* UVCollisionPresenter::CreateConvex(USceneComponent* Parent, const FName& Name, const TArray<FVector>& Vertices)
{
	if (!OwnerActor || !Parent)
	{
		return nullptr;
	}

	UProceduralMeshComponent* Comp = NewObject<UProceduralMeshComponent>(OwnerActor.Get(), Name);
	Comp->SetupAttachment(Parent);
	Comp->RegisterComponent();
	Comp->SetVisibility(false, true);
	Comp->bUseComplexAsSimpleCollision = false;
	Comp->ClearCollisionConvexMeshes();
	Comp->AddCollisionConvexMesh(Vertices);
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
	Prim->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Prim->SetGenerateOverlapEvents(false);
	Prim->PrimaryComponentTick.bCanEverTick = false;
}

bool UVCollisionPresenter::HasAnyPrimitives(const FVCollisionInstance& Instance)
{
	return Instance.BoxComponents.Num() > 0
		|| Instance.SphereComponents.Num() > 0
		|| Instance.CapsuleComponents.Num() > 0
		|| Instance.ConvexComponents.Num() > 0;
}
