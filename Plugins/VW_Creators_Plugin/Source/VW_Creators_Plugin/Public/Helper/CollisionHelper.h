#pragma once

#include "CoreMinimal.h"
#include "Model/Package/MPCollision.h"

class VW_CREATORS_PLUGIN_API FCPCollisionHelper
{
public:
	static bool Initialize(bool InShowLogs);
	static bool IsCollisionExist(const FGuid& InMeshID);
	static void AddCollisionItem(const FGuid& InMeshID, const FMPCollision& InCollision);
	static bool GetCollisionItem(const FGuid& InMeshID, FMPCollision& OutCollision);
	static TArray<FMPCollision> GetCollisionItems();
	static bool SerializeCollisionItem(const UStaticMesh* Mesh, FMPCollision& OutCollision);
private:
	static bool ShowLogs;
	static TMap<FGuid, FMPCollision> SharedCollisionMap;
};
