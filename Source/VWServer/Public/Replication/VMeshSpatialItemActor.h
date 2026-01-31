#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/VSpatialItemActorInterface.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "VMeshSpatialItemActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class VWSERVER_API AVMeshSpatialItemActor : public AActor, public ISpatialItemActorInterface, public ISpatialMeshItemActorInterface
{
	GENERATED_BODY()

public:
	AVMeshSpatialItemActor();

	void InitializeFromItem(const FVMRepMeshSpatialItem& InItem);
	void UpdateFromItem(const FVMRepMeshSpatialItem& InItem);

	UStaticMeshComponent* GetMeshComponent() const;
	const FSpatialItemId& GetSpatialItemId() const override;
	const FMeshAssetId& GetMeshAssetId() const;
	const TArray<uint32>& GetMaterialIdsBySlot() const;

	virtual void PostNetInit() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual const FMeshAssetId& GetSpatialMeshAssetId() const override;
	virtual const TArray<uint32>& GetSpatialMaterialIdsBySlot() const override;
	virtual UStaticMeshComponent* GetSpatialMeshComponent() const override;

private:
	static FTransform ToTransform(const FVMTransformNet& InTransform);
	void ApplyTransform();

	UFUNCTION()
	void OnRep_SpatialTransform();

	UFUNCTION()
	void OnRep_MeshAssetId();

	UFUNCTION()
	void OnRep_MaterialIdsBySlot();

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(Replicated)
	FSpatialItemId SpatialItemId;

	UPROPERTY(ReplicatedUsing = OnRep_SpatialTransform)
	FVMTransformNet ReplicatedTransform;

	UPROPERTY(ReplicatedUsing = OnRep_MeshAssetId)
	FMeshAssetId MeshAssetId;

	UPROPERTY(ReplicatedUsing = OnRep_MaterialIdsBySlot)
	TArray<uint32> MaterialIdsBySlot;

	UPROPERTY(Replicated)
	bool bHasFlags = false;

	UPROPERTY(Replicated)
	uint8 Flags = 0;
};
