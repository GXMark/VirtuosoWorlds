#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Model/Package/VMCollision.h"
#include "VCollisionPresenter.generated.h"

class USceneComponent;
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;
class UProceduralMeshComponent;

USTRUCT()
struct FVCollisionInstance
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USceneComponent> ItemRoot = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UBoxComponent>> BoxComponents;

	UPROPERTY()
	TArray<TObjectPtr<USphereComponent>> SphereComponents;

	UPROPERTY()
	TArray<TObjectPtr<UCapsuleComponent>> CapsuleComponents;

	UPROPERTY()
	TArray<TObjectPtr<UProceduralMeshComponent>> ConvexComponents;

	UPROPERTY()
	FGuid CollisionId;

	FVCollisionInstance()
		: CollisionId(FGuid(0,0,0,0))
	{}
};

/**
 * Owns per-item collision components (box/sphere/capsule) under a single collision root.
 * Collision definitions are streamed as shared FVMCollision objects keyed by collision id.
 */
UCLASS()
class VWCLIENT_API UVCollisionPresenter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AActor* InOwner, USceneComponent* InCollisionRoot);

	// Called when a spatial item arrives/updates.
	void OnItemUpsert(const FGuid& ItemId, const FTransform& WorldTransform, const FGuid& CollisionId);

	// Called when shared collision definitions arrive.
	void SubmitCollisionDefs(const TArray<FVMCollision>& Collisions);

	// Called when a spatial item is removed/unloaded.
	void OnItemRemoved(const FGuid& ItemId);

private:
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY()
	TObjectPtr<USceneComponent> CollisionRoot = nullptr;

	UPROPERTY()
	TMap<FGuid, FVMCollision> CollisionDefsById;

	UPROPERTY()
	TSet<FGuid> LoadedCollisionIds;

	UPROPERTY()
	TMap<FGuid, FTransform> ItemWorldTransformById;

	UPROPERTY()
	TMap<FGuid, FGuid> CollisionIdByItemId;

	UPROPERTY()
	TMap<FGuid, FVCollisionInstance> InstancesByItemId;

private:
	void RebuildItem(const FGuid& ItemId);
	void DestroyPrimitives(FVCollisionInstance& Instance);
	USceneComponent* EnsureItemRoot(FVCollisionInstance& Instance, const FGuid& ItemId);

	UBoxComponent* CreateBox(USceneComponent* Parent, const FName& Name);
	USphereComponent* CreateSphere(USceneComponent* Parent, const FName& Name);
	UCapsuleComponent* CreateCapsule(USceneComponent* Parent, const FName& Name);
	UProceduralMeshComponent* CreateConvex(USceneComponent* Parent, const FName& Name, const TArray<FVector>& Vertices);

	static void ConfigureCollision(UPrimitiveComponent* Prim);
	static bool HasAnyPrimitives(const FVCollisionInstance& Instance);
};
