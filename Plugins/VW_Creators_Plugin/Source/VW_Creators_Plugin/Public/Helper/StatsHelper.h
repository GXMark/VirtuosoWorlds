#pragma once

#include "CoreMinimal.h"
#include "Model/Package/MPAssetRef.h"

struct FMeshGroupInfo {
	FMPAssetRef MeshRef;
	int32 InstanceCount;
	int32 MaterialCount;
};

class VW_CREATORS_PLUGIN_API FCPStatsHelper
{
public:
	static void Initialize();
	static void Summary();
	static int32 TotalActors;
	static int32 TotalMeshes;
	static int32 TotalMaterials;
	static int32 TotalTextures;
	static int32 TotalSceneActors;
	static int32 TotalSpotLights;
	static int32 TotalPointLights;
	static int32 TotalDecals;
	static TMap<uint64, FMeshGroupInfo> MeshGroups;

	static void UpdateMeshGroups(const FMPAssetRef& InMeshRef, const TArray<FMPAssetRef>& InMaterialRefs);
};
