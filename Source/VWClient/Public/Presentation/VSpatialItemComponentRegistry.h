#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VSpatialItemComponentRegistry.generated.h"

class USceneComponent;

/**
 * Shared registry for per-item scene roots under PresentationRoot.
 *
 * Scale semantics (Model A):
 * - Item roots carry the full spatial transform (including scale).
 * - Collision primitives remain at relative scale (1,1,1); sizes come from shape params.
 * - Collision data is authored in unscaled local space; item root scale applies uniformly.
 * - Non-uniform scale is allowed, but spheres/capsules will inherit it (UE may distort them).
 */
UCLASS()
class VWCLIENT_API UVSpatialItemComponentRegistry : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwner, USceneComponent* InPresentationRoot);

	USceneComponent* GetOrCreateItemRoot(const FGuid& ItemId);
	USceneComponent* FindItemRoot(const FGuid& ItemId) const;

	void DestroyItemRoot(const FGuid& ItemId);
	void DestroyAllItemRoots();

private:
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor = nullptr;

	TWeakObjectPtr<USceneComponent> PresentationRoot;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<USceneComponent>> ItemRootsById;
};
