#include "Helper/StatsHelper.h"
#include "Model/Package/MPAssetRef.h"

int32 FCPStatsHelper::TotalActors = 0;
int32 FCPStatsHelper::TotalMeshes = 0;
int32 FCPStatsHelper::TotalMaterials = 0;
int32 FCPStatsHelper::TotalTextures = 0;
int32 FCPStatsHelper::TotalSceneActors = 0;
int32 FCPStatsHelper::TotalSpotLights = 0;
int32 FCPStatsHelper::TotalPointLights = 0;
int32 FCPStatsHelper::TotalDecals = 0;
TMap<uint64, FMeshGroupInfo> FCPStatsHelper::MeshGroups;

// Initialize build statistics
void FCPStatsHelper::Initialize()
{
	TotalActors = 0;
	TotalMeshes = 0;
	TotalMaterials = 0;
	TotalTextures = 0;
	TotalSpotLights = 0;
	TotalPointLights = 0;
	TotalDecals = 0;	
	MeshGroups.Empty();
}

void FCPStatsHelper::UpdateMeshGroups(const FMPAssetRef& InMeshRef, const TArray<FMPAssetRef>& InMaterialRefs)
{
	// Compute the hash key
	uint64 Hash = GetTypeHash(InMeshRef.id);
	for (const FMPAssetRef& MatRef : InMaterialRefs)
		Hash = HashCombineFast(Hash, GetTypeHash(MatRef.id));
	
	if (!MeshGroups.Contains(Hash))
		MeshGroups.Add(Hash, {});
	
	MeshGroups[Hash].MeshRef = InMeshRef;
	MeshGroups[Hash].InstanceCount++;
	MeshGroups[Hash].MaterialCount = InMaterialRefs.Num();
}

// Show the build package summary
void FCPStatsHelper::Summary()
{
	UE_LOG(LogTemp, Log, TEXT("VWCP - [ Build Summary ]"));
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Actors:       %d ]"), TotalActors);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Mesh:         %d ]"), TotalMeshes);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Materials:    %d ]"), TotalMaterials);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Textures:     %d ]"), TotalTextures);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Decals:       %d ]"), TotalDecals);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Scene Actors: %d ]"), TotalSceneActors);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Spot Lights:  %d ]"), TotalSpotLights);
	UE_LOG(LogTemp, Log, TEXT("VWCP - [   Total Point Lights: %d ]"), TotalPointLights);
	int32 Index=0;
	
	TArray<FMeshGroupInfo> MeshGroupArray;
	MeshGroups.GenerateValueArray(MeshGroupArray);
	MeshGroupArray.Sort([](const FMeshGroupInfo& A, const FMeshGroupInfo& B) { return A.InstanceCount > B.InstanceCount; });
	
	for (FMeshGroupInfo Info : MeshGroupArray)
		UE_LOG(LogTemp, Log, TEXT("VWCP - [   Mesh Group: %05d, Mesh Name: %-50s, Instances: %05d, Material Count: %02d ]"), 
			Index++, *Info.MeshRef.name.ToString(), Info.InstanceCount, Info.MaterialCount);
}