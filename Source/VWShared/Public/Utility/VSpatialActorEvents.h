#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMRepSpatialItemNet.h"

class AActor;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpatialActorPostInit, AActor* /*Actor*/, const FSpatialItemId& /*SpatialId*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpatialActorMeshAssetIdChanged, AActor* /*Actor*/, const FMeshAssetId& /*MeshAssetId*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpatialActorMaterialIdsChanged, AActor* /*Actor*/, const TArray<uint32>& /*MaterialIds*/);

class VWSHARED_API FSpatialActorEvents
{
public:
	static FOnSpatialActorPostInit& OnSpatialActorPostInit();
	static FOnSpatialActorMeshAssetIdChanged& OnSpatialActorMeshAssetIdChanged();
	static FOnSpatialActorMaterialIdsChanged& OnSpatialActorMaterialIdsChanged();
};
