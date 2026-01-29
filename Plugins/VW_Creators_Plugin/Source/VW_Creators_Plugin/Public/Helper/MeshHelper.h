#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Model/Package/MPActor.h"
#include "Model/Package/MPMaterial.h"

class VW_CREATORS_PLUGIN_API FPMeshHelper
{
public:
	static bool Initialize(bool InVerbose);
	static bool SerializeMeshComponent(const UStaticMeshComponent* InComponent, FMPActor& OutActorItem, TArray<FMPMaterial>& OutMaterialDataArray);
	static void GetHISMInstances(const UHierarchicalInstancedStaticMeshComponent* InHISMComponent, TArray<FTransform>& OutInstances);

private:
	static bool ShowOutputLog;
};
