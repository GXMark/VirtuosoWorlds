#include "Helper/MeshHelper.h"
#include "Helper/MaterialHelper.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Constant/Constants.h"
#include "Engine/StaticMesh.h"
#include "Helper/CacheHelper.h"
#include "Misc/FileHelper.h"
#include "Helper/CollisionHelper.h"
#include "Helper/StatsHelper.h"

// Initialize
bool FPMeshHelper::ShowOutputLog = false;

// Initialize mesh item helper
bool FPMeshHelper::Initialize(const bool InVerbose)
{
	ShowOutputLog = InVerbose;

	return true;
}

// Serialize the mesh component
bool FPMeshHelper::SerializeMeshComponent(const UStaticMeshComponent* InComponent, FMPActor& OutActorItem, TArray<FMPMaterial>& OutMaterialDataArray)
{
	// Check that a static mesh exists
	if (TObjectPtr<UStaticMesh> StaticMesh = InComponent->GetStaticMesh())
	{
		FMPMeshComponent SMComponent;

		FGuid MeshID = StaticMesh->GetLightingGuid();

		FMPAssetRef MeshRef = FMPAssetRef(MeshID, FName(StaticMesh->GetName()), FPAssetType::Mesh, FName(StaticMesh->GetPathName()), NAME_None);
		
		FPCacheHelper::AddAssetRef(MeshRef);

		// Assign an already allocated shared mesh GUID
		SMComponent.mesh_ref = MeshRef;

		// Does the collision primitive(s) exist for the mesh
		if (!FCPCollisionHelper::IsCollisionExist(MeshID))
		{
			if (FMPCollision CollisionItem; FCPCollisionHelper::SerializeCollisionItem(StaticMesh, CollisionItem))
			{
				CollisionItem.MeshID = MeshID;
				FCPCollisionHelper::AddCollisionItem(MeshID, CollisionItem);
			}
		}

		TArray<UMaterialInterface*> Materials;
		Materials = InComponent->GetMaterials();

		SMComponent.material_refs.Empty();

		for (int32 MaterialIndex = 0; MaterialIndex < Materials.Num(); MaterialIndex++)
		{
			FMPMaterial MaterialItem;
			if (!FCPMaterialHelper::GetMaterialItem(Materials[MaterialIndex], MaterialItem))
			{
				// Serialize the material
				FCPMaterialHelper::SerializeMaterial(Materials[MaterialIndex], MaterialItem);

				// Add the shared material
				FCPMaterialHelper::AddMaterial(Materials[MaterialIndex], MaterialItem);

				OutMaterialDataArray.Add(MaterialItem);
			}
			
			SMComponent.material_refs.Add(FMPAssetRef(MaterialItem.id, MaterialItem.name, FPAssetType::Material, FName(Materials[MaterialIndex]->GetPathName()), NAME_None));
		}

		OutActorItem.mesh_comp = SMComponent;
		
		FCPStatsHelper::UpdateMeshGroups(SMComponent.mesh_ref, SMComponent.material_refs);
		
		return true;
	}

	return false;
}

// Get hierarchical transform instances
void FPMeshHelper::GetHISMInstances(const UHierarchicalInstancedStaticMeshComponent* InHISMComponent, TArray<FTransform>& OutInstances)
{
	// Get the number of instances
	int32 InstanceCount = InHISMComponent->GetInstanceCount();

	// Assign the transform instances
	for (int32 InstanceIndex = 0; InstanceIndex < InstanceCount; ++InstanceIndex)
	{
		FTransform InstanceWorldTransform;

		// Retrieve the local transform of each instance
		if (InHISMComponent->GetInstanceTransform(InstanceIndex, InstanceWorldTransform, true))
		{
			OutInstances.Add(InstanceWorldTransform);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get transform for instance %d"), InstanceIndex);
		}
	}
}
