#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Components/SceneComponent.h"
#include "Subsystem/VAssetManager.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "VItemPresenter.generated.h"

class UStaticMeshComponent;

UCLASS()
class VWCLIENT_API UVItemPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		AActor* InOwner,
		USceneComponent* InPresentationRoot,
		UVAssetManager* InAssetManager);
	// Present a batch of spatial items received from the server
	void PresentSpatialItemsBatch(const TArray<FVMSpatialItemNet>& Items);
private:
	TWeakObjectPtr<AActor> PresentationOwner;
	TWeakObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TObjectPtr<UVAssetManager> AssetManager;
	TMap<FGuid, TObjectPtr<USceneComponent>> SpawnedComponents;
	TMap<FGuid, FGuid> RequestedMeshByItemId;
	TMap<FGuid, TArray<FGuid>> RequestedMaterialsByItemId;

	void OnItemAdded(const FVMSpatialItemNet& InItem);
	void OnItemUpdated(const FVMSpatialItemNet& InItem);
	void OnItemRemoved(const FGuid& InItemId);

	void SpawnOrUpdateMesh(
		const FGuid& InItemId,
		const FVMMeshComponentNet& InMeshData,
		const FTransform& InWorldTransform,
		const FGuid& InParentId);

	void ApplyMaterialsAsync(
		const FGuid& InItemId,
		UStaticMeshComponent* MeshComp,
		const TArray<FVMGuidNet>& InMaterialIds);

	void DestroyItem(const FGuid& InItemId);
};
