#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Components/SceneComponent.h"
#include "Replication/VReplicatedChunk.h"
#include "Subsystem/VAssetManager.h"
#include "VChunkPresenter.generated.h"

UCLASS()
class VWCLIENT_API UVChunkPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		AActor* InOwner,
		USceneComponent* InPresentationRoot,
		UVAssetManager* InAssetManager);

	void ObserveChunk(AVReplicatedChunk* InChunk);
	
private:
	TWeakObjectPtr<AActor> PresentationOwner;
	TWeakObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TObjectPtr<UVAssetManager> AssetManager;

	TSet<TWeakObjectPtr<AVReplicatedChunk>> ObservedChunks;

	TMap<FGuid, TObjectPtr<USceneComponent>> SpawnedComponents;
	TMap<FGuid, FGuid> RequestedMeshByItemId;

	void OnItemAdded(const FVMSpatialItemNet& InItem);
	void OnItemUpdated(const FVMSpatialItemNet& InItem);
	void OnItemRemoved(const FGuid& InItemId);

	void SpawnOrUpdateMesh(
		const FGuid& InItemId,
		const FVMMeshComponentNet& InMeshData,
		const FTransform& InWorldTransform,
		const FGuid& InParentId);

	void DestroyItem(const FGuid& InItemId);
};
