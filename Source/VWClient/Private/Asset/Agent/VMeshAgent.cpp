#include "Asset/Agent/VMeshAgent.h"
#include "Engine/StaticMesh.h"
#include "Model/Constant/VConstants.h"
#include "Subsystem/VAssetManager.h"

// Initialize mesh component
bool UVMeshAgent::Initialize()
{
	SharedMeshMap.Empty();
	
	return true;
}

// Is the mesh locally cached (memory or disk)
bool UVMeshAgent::IsLocallyCached(const FGuid& InAssetID, const FName InCacheType) const
{
	if (InCacheType == FVCacheType::Memory || InCacheType == FVCacheType::Local)
	{
		if (SharedMeshMap.Contains(InAssetID))
			return true;
	}

	if (InCacheType == FVCacheType::Disk || InCacheType == FVCacheType::Local)
	{
		const FString FilePath = UVAssetManager::CachePath / InAssetID.ToString() + TEXT(".mesh");
		return FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	}

	return false;
}

// Remove all meshes from memory
void UVMeshAgent::RemoveAll()
{
	SharedMeshMap.Empty();
}

// Get all mesh asset identifiers
TArray<FGuid> UVMeshAgent::GetMeshIDs() const
{
	TArray<FGuid> AssetIDs;
	SharedMeshMap.GenerateKeyArray(AssetIDs);
	return AssetIDs;
}

// Set mesh by mesh asset identifier
void UVMeshAgent::SetMesh(const FGuid& InMeshID, UStaticMesh* InMesh)
{
	if (!InMesh)
		return;

	SharedMeshMap.Add(InMeshID, InMesh);
}

// Get the mesh by its mesh identifier
UStaticMesh* UVMeshAgent::GetMesh(const FGuid& InMeshID)
{
	if (SharedMeshMap.Contains(InMeshID))
		return SharedMeshMap[InMeshID];
	return nullptr;
}

/*/**
 * Update the HISM local transform instances
 * @param InHISMItem - Hierarchical Instance Static Mesh (HISM) Item
 * @param OutHISMComponent - HISM component
 * @return true - success, false - fail
 #1#
bool UMeshComp::DeserializeHISMItem(const FMHISMComponent& InHISMItem, UHierarchicalInstancedStaticMeshComponent* OutHISMComponent) const
{
	// Only proceed if the mesh exists
	if (SharedMeshMap.Contains(InHISMItem.asset_ref.id))
	{
		// Assign the static mesh
		OutHISMComponent->SetStaticMesh(SharedMeshMap[InHISMItem.asset_ref.id]);
		
		// Create the local transforms
		OutHISMComponent->ClearInstances();
		for (FMTransform LocalTransform : InHISMItem.instance_transforms)
		{
			FRotator Rot(LocalTransform.rotation.X, LocalTransform.rotation.Z, LocalTransform.rotation.Y);
			FTransform Transform = FTransform(Rot, LocalTransform.position, LocalTransform.scale);
			OutHISMComponent->AddInstance(Transform);
		}
		return true;
	}
	return false;
}*/

// Has mesh distance field data
bool UVMeshAgent::HasDistanceFieldData(UStaticMesh* Mesh)
{
	if (!Mesh)
	{
		return false;
	}

	// Get RenderData
	FStaticMeshRenderData* RenderData = Mesh->GetRenderData();
	if (!RenderData)
	{
		return false;
	}

	// Distance field data is per LOD
	for (const FStaticMeshLODResources& LOD : RenderData->LODResources)
	{
		if (LOD.DistanceFieldData)
		{
			return true; // Found DF data
		}
	}

	return false; // No distance field data
}

// Has the mesh ray tracing geometry data
bool UVMeshAgent::HasRayTracingGeometry(UStaticMesh* Mesh)
{
    if (!Mesh)
    {
        return false;
    }

    // Make sure RenderData exists
    FStaticMeshRenderData* RenderData = Mesh->GetRenderData();
    if (!RenderData)
    {
        return false;
    }

    // Check all LODs for RayTracingGeometry
    for (int32 LODIndex = 0; LODIndex < RenderData->LODResources.Num(); ++LODIndex)
    {
        const FStaticMeshLODResources& LOD = RenderData->LODResources[LODIndex];
        if (LOD.RayTracingGeometry)
        {
            return true; // Valid RT geometry found
        }
    }

    return false; // No RT geometry
}