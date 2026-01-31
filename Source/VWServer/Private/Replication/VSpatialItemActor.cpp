#include "Replication/VSpatialItemActor.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/NetConnection.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/ConvexElem.h"
#include "PhysicsEngine/SphylElem.h"
#include "PhysicsEngine/SphereElem.h"
#include "ProceduralMeshComponent.h"
#include "VWDatabase/Public/VDataService.h"
#include "JsonObjectConverter.h"
#include "Replication/VReplicationManager.h"

namespace
{
	static TMap<FGuid, FVMCollision> GCollisionCache;

	void BuildAggregateGeom(const FVMCollision& Def, FKAggregateGeom& OutAggGeom)
	{
		OutAggGeom.SphereElems.Reset();
		OutAggGeom.BoxElems.Reset();
		OutAggGeom.SphylElems.Reset();
		OutAggGeom.ConvexElems.Reset();

		for (const FVMSphereCollision& Sphere : Def.spheres)
		{
			FKSphereElem Elem;
			Elem.Center = Sphere.center;
			Elem.Radius = Sphere.radius;
			OutAggGeom.SphereElems.Add(Elem);
		}

		for (const FVMBoxCollision& Box : Def.boxes)
		{
			FKBoxElem Elem;
			Elem.Center = Box.Center;
			Elem.Rotation = Box.Rotation;
			Elem.X = Box.Extents.X;
			Elem.Y = Box.Extents.Y;
			Elem.Z = Box.Extents.Z;
			OutAggGeom.BoxElems.Add(Elem);
		}

		for (const FVMCapsuleCollision& Capsule : Def.capsules)
		{
			FKSphylElem Elem;
			Elem.Center = Capsule.center;
			Elem.Rotation = Capsule.rotation;
			Elem.Radius = Capsule.radius;
			Elem.Length = Capsule.length;
			OutAggGeom.SphylElems.Add(Elem);
		}

		for (const FVMConvexCollision& Convex : Def.convexes)
		{
			if (Convex.vertices.Num() < 4)
			{
				continue;
			}

			FKConvexElem Elem;
			Elem.VertexData = Convex.vertices;
			Elem.UpdateElemBox();
			OutAggGeom.ConvexElems.Add(MoveTemp(Elem));
		}
	}

	bool LoadCollisionDef(UWorld* World, const FGuid& CollisionId, FVMCollision& OutCollision)
	{
		if (!World || !CollisionId.IsValid())
		{
			return false;
		}

		if (const FVMCollision* Cached = GCollisionCache.Find(CollisionId))
		{
			OutCollision = *Cached;
			return true;
		}

		UVDataService* DataService = UVDataService::Get(World);
		if (!DataService)
		{
			return false;
		}

		const FString Json = DataService->GetCollision(CollisionId.ToString());
		if (Json.IsEmpty())
		{
			return false;
		}

		FVMCollision Parsed;
		if (!FJsonObjectConverter::JsonObjectStringToUStruct<FVMCollision>(Json, &Parsed, 0, 0))
		{
			return false;
		}

		GCollisionCache.Add(CollisionId, Parsed);
		OutCollision = Parsed;
		return true;
	}

	void ConfigureCollisionComponent(UPrimitiveComponent* Prim)
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
}

AVSpatialItemActor::AVSpatialItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(false);
	bAlwaysRelevant = false;

	CollisionRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionRoot"));
	SetRootComponent(CollisionRoot);
}

void AVSpatialItemActor::InitializeSpatialItem(const FVMSpatialItemNet& InItem)
{
	SpatialItem = InItem;
	ApplyCollisionFromItem();
}

void AVSpatialItemActor::UpdateSpatialItem(const FVMSpatialItemNet& InItem)
{
	SpatialItem = InItem;
	ApplyCollisionFromItem();
}

const FVMSpatialItemNet& AVSpatialItemActor::GetSpatialItem() const
{
	return SpatialItem;
}

bool AVSpatialItemActor::IsNetRelevantFor(
	const AActor* RealViewer,
	const AActor* ViewTarget,
	const FVector& SrcLocation) const
{
	const UWorld* World = GetWorld();
	const UVReplicationManager* ReplicationManager = World ? World->GetSubsystem<UVReplicationManager>() : nullptr;
	const UNetConnection* Connection = RealViewer ? RealViewer->GetNetConnection() : nullptr;

	if (!ReplicationManager || !Connection || !ReplicationManager->HasConnectionRelevancy(Connection))
	{
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	}

	return ReplicationManager->IsSpatialItemRelevantForConnection(SpatialItem.ItemID.Value, Connection);
}

void AVSpatialItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVSpatialItemActor, SpatialItem);
}

void AVSpatialItemActor::ApplyCollisionFromItem()
{
	const FGuid CollisionId = SpatialItem.CollisionID.Value;

	if (!CollisionId.IsValid())
	{
		AppliedCollisionId.Invalidate();
		bAppliedToBodySetup = false;
		ClearCollisionComponents();
		return;
	}

	if (AppliedCollisionId == CollisionId && (bAppliedToBodySetup || CollisionComponents.Num() > 0))
	{
		return;
	}

	FVMCollision CollisionDef;
	if (!LoadCollisionDef(GetWorld(), CollisionId, CollisionDef))
	{
		AppliedCollisionId.Invalidate();
		bAppliedToBodySetup = false;
		ClearCollisionComponents();
		return;
	}

	if (UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>())
	{
		if (UBodySetup* BodySetup = MeshComp->GetBodySetup())
		{
			BuildAggregateGeom(CollisionDef, BodySetup->AggGeom);
			BodySetup->CollisionTraceFlag = CTF_UseSimpleAsComplex;
			BodySetup->InvalidatePhysicsData();
			BodySetup->CreatePhysicsMeshes();
			MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			MeshComp->SetGenerateOverlapEvents(false);
			MeshComp->RecreatePhysicsState();
			AppliedCollisionId = CollisionId;
			bAppliedToBodySetup = true;
			ClearCollisionComponents();
			return;
		}
	}

	ClearCollisionComponents();
	bAppliedToBodySetup = false;

	if (!CollisionRoot)
	{
		return;
	}

	for (int32 i = 0; i < CollisionDef.boxes.Num(); ++i)
	{
		const FVMBoxCollision& Box = CollisionDef.boxes[i];
		UBoxComponent* Comp = NewObject<UBoxComponent>(this, *FString::Printf(TEXT("Server_Box_%d"), i));
		if (!Comp) continue;

		Comp->SetupAttachment(CollisionRoot);
		Comp->SetRelativeLocation(Box.Center);
		Comp->SetRelativeRotation(Box.Rotation);
		Comp->SetBoxExtent(Box.Extents, true);
		Comp->RegisterComponent();
		ConfigureCollisionComponent(Comp);
		CollisionComponents.Add(Comp);
	}

	for (int32 i = 0; i < CollisionDef.spheres.Num(); ++i)
	{
		const FVMSphereCollision& Sphere = CollisionDef.spheres[i];
		USphereComponent* Comp = NewObject<USphereComponent>(this, *FString::Printf(TEXT("Server_Sphere_%d"), i));
		if (!Comp) continue;

		Comp->SetupAttachment(CollisionRoot);
		Comp->SetRelativeLocation(Sphere.center);
		Comp->SetSphereRadius(Sphere.radius, true);
		Comp->RegisterComponent();
		ConfigureCollisionComponent(Comp);
		CollisionComponents.Add(Comp);
	}

	for (int32 i = 0; i < CollisionDef.capsules.Num(); ++i)
	{
		const FVMCapsuleCollision& Capsule = CollisionDef.capsules[i];
		UCapsuleComponent* Comp = NewObject<UCapsuleComponent>(this, *FString::Printf(TEXT("Server_Capsule_%d"), i));
		if (!Comp) continue;

		Comp->SetupAttachment(CollisionRoot);
		Comp->SetRelativeLocation(Capsule.center);
		Comp->SetRelativeRotation(Capsule.rotation);
		Comp->SetCapsuleSize(Capsule.radius, Capsule.length, true);
		Comp->RegisterComponent();
		ConfigureCollisionComponent(Comp);
		CollisionComponents.Add(Comp);
	}

	for (int32 i = 0; i < CollisionDef.convexes.Num(); ++i)
	{
		const FVMConvexCollision& Convex = CollisionDef.convexes[i];
		if (Convex.vertices.Num() < 4)
		{
			continue;
		}

		UProceduralMeshComponent* Comp = NewObject<UProceduralMeshComponent>(this, *FString::Printf(TEXT("Server_Convex_%d"), i));
		if (!Comp) continue;

		Comp->SetupAttachment(CollisionRoot);
		Comp->SetRelativeScale3D(FVector::OneVector);
		Comp->RegisterComponent();
		Comp->SetVisibility(false, true);
		Comp->bUseComplexAsSimpleCollision = false;
		Comp->ClearCollisionConvexMeshes();
		Comp->AddCollisionConvexMesh(Convex.vertices);
		ConfigureCollisionComponent(Comp);
		CollisionComponents.Add(Comp);
	}

	AppliedCollisionId = CollisionId;
	bAppliedToBodySetup = false;
}

void AVSpatialItemActor::ClearCollisionComponents()
{
	for (UPrimitiveComponent* Comp : CollisionComponents)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	CollisionComponents.Reset();
}
