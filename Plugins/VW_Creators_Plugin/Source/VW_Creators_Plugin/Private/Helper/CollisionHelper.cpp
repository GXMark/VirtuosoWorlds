#include "Helper/CollisionHelper.h"
#include "Model/Package/MPCollision.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/ConvexElem.h"
#include "PhysicsEngine/SphereElem.h"
#include "PhysicsEngine/SphylElem.h"
#include "PhysicsEngine/BodySetup.h"

bool FCPCollisionHelper::ShowLogs = false;
TMap<FGuid, FMPCollision> FCPCollisionHelper::SharedCollisionMap = {};

// Initialize the collision helper
bool FCPCollisionHelper::Initialize(bool InShowLogs)
{
	ShowLogs = InShowLogs;
	SharedCollisionMap.Empty();
	return true;
}

// Does the collision item exist by mesh identifier
bool FCPCollisionHelper::IsCollisionExist(const FGuid& InMeshID)
{
	return SharedCollisionMap.Contains(InMeshID);
}

// Add a collision item by mesh identifier
void FCPCollisionHelper::AddCollisionItem(const FGuid& InMeshID, const FMPCollision& InCollision)
{
	SharedCollisionMap.Add(InMeshID, InCollision);
}

// Get a colllision item by mesh identifier
bool FCPCollisionHelper::GetCollisionItem(const FGuid& InMeshID, FMPCollision& OutCollision)
{
	if (const FMPCollision* Found = SharedCollisionMap.Find(InMeshID))
	{
		OutCollision = *Found;
		return true;
	}
	return false;
}

// Get all collision items
TArray<FMPCollision> FCPCollisionHelper::GetCollisionItems()
{
	TArray<FMPCollision> CollisionItems;
	SharedCollisionMap.GenerateValueArray(CollisionItems);
	return CollisionItems;
}

// Serialize the collision item from the static mesh
bool FCPCollisionHelper::SerializeCollisionItem(const UStaticMesh* Mesh, FMPCollision& InCollisionItem)
{
	if (!Mesh || !Mesh->GetBodySetup())
		return false;

	const FKAggregateGeom& Agg = Mesh->GetBodySetup()->AggGeom;

	/* -------- Spheres -------- */
	for (const FKSphereElem& S : Agg.SphereElems)
	{
		FMPSphereCollision& D = InCollisionItem.Spheres.AddDefaulted_GetRef();
		D.Center = S.Center;
		D.Radius = S.Radius;
	}

	/* -------- Boxes -------- */
	for (const FKBoxElem& B : Agg.BoxElems)
	{
		FMPBoxCollision& D = InCollisionItem.Boxes.AddDefaulted_GetRef();
		D.Center = B.Center;
		D.Rotation = B.Rotation;
		D.Extents = FVector(B.X, B.Y, B.Z);
	}

	/* -------- Capsules -------- */
	for (const FKSphylElem& C : Agg.SphylElems)
	{
		FMPCapsuleCollision& D = InCollisionItem.Capsules.AddDefaulted_GetRef();
		D.Center = C.Center;
		D.Rotation = C.Rotation;
		D.Radius = C.Radius;
		D.Length = C.Length;
	}

	/* -------- Convex -------- */
	for (const FKConvexElem& C : Agg.ConvexElems)
	{
		FMPConvexCollision& D = InCollisionItem.Convexes.AddDefaulted_GetRef();
		D.Vertices = C.VertexData; // DO NOT reorder
	}

	return true;
}
