#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Network/VMMeshComponentNet.h"
#include "VMeshPresenter.generated.h"

class UStaticMeshComponent;
class UVAssetManager;

UCLASS()
class VWCLIENT_API UVMeshPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwner, USceneComponent* InPresentationRoot, UVAssetManager* InAssetManager);
	UStaticMeshComponent* PresentMeshItem(
		const FGuid& InItemId,
		const FVMMeshComponentNet& InMeshData,
		const FTransform& InWorldTransform,
		const FGuid& InParentId);
	UStaticMeshComponent* FindMeshComponent(const FGuid& InItemId) const;
	void DestroyItem(const FGuid& InItemId);

private:
	TWeakObjectPtr<AActor> PresentationOwner;
	TWeakObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TObjectPtr<UVAssetManager> AssetManager;

	TMap<FGuid, TObjectPtr<UStaticMeshComponent>> SpawnedComponents;
	TMap<FGuid, FGuid> RequestedMeshByItemId;
};
