#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
// NOTE: These types are stored by value in stream state and must be complete in this header.
#include "Model/Network/VMSpatialItemNet.h"
#include "Model/Package/VMMaterial.h"
#include "Model/Package/VMCollision.h"
#include "VRegionServerBridge.generated.h"


/**
 * Server-side bridge object that centralizes region streaming logic and database access.
 *
 * Header lives in VWShared so server code can be referenced by transport classes (e.g. PlayerController)
 * without creating module cycles. Implementation lives in VWServer.
 */
UCLASS()
class VWSERVER_API URegionServerBridge : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UWorld* InWorld);

	// Spatial
	void HandleSpatialRequest(
		APlayerController* PC,
		const FVector& Origin,
		int32 MaxItems);

	// Materials
	void HandleMaterialsRequest(
		APlayerController* PC,
		const TArray<FGuid>& MaterialIds);

	// Collisions
	void HandleCollisionsRequest(
		APlayerController* PC,
		const TArray<FGuid>& CollisionIds);

private:
	UPROPERTY()
	TObjectPtr<UWorld> World;

	// Per-connection stream state
	struct FSpatialStreamState
	{
		TArray<FVMSpatialItemNet> AllItems;
		TSet<FGuid> DeliveredItemIds;
		bool bInitialized = false;
	};

	TMap<TWeakObjectPtr<APlayerController>, FSpatialStreamState> SpatialStreams;

	void EnsureSpatialState(APlayerController* PC);
	FVMTransformNet MakeNetTransform(const FVMTransform& In) const;
	FVMMeshComponentNet MakeMeshNet(const FVMMeshComponent& In) const;
	FVMPointLightComponentNet MakePointLightNet(const FVMPointLightComponent& In) const;
	FVMSpotLightComponentNet MakeSpotLightNet(const FVMSpotLightComponent& In) const;
	FVMDecalComponentNet MakeDecalNet(const FVMDecalComponent& In) const;
	void AddMeshSpatialItem(const FVMActor& Actor, FSpatialStreamState& State);
	void AddPointLightSpatialItem(const FVMActor& Actor, FSpatialStreamState& State);
	void AddSpotLightSpatialItem(const FVMActor& Actor, FSpatialStreamState& State);
	void AddDecalSpatialItem(const FVMActor& Actor, FSpatialStreamState& State);
};
