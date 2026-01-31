#include "Utility/VSpatialActorEvents.h"

FOnSpatialActorPostInit& FSpatialActorEvents::OnSpatialActorPostInit()
{
	static FOnSpatialActorPostInit Delegate;
	return Delegate;
}

FOnSpatialActorMeshAssetIdChanged& FSpatialActorEvents::OnSpatialActorMeshAssetIdChanged()
{
	static FOnSpatialActorMeshAssetIdChanged Delegate;
	return Delegate;
}

FOnSpatialActorMaterialIdsChanged& FSpatialActorEvents::OnSpatialActorMaterialIdsChanged()
{
	static FOnSpatialActorMaterialIdsChanged Delegate;
	return Delegate;
}
