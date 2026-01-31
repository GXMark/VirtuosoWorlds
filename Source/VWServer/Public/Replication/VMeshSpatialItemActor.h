#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "VMeshSpatialItemActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class VWSERVER_API AVMeshSpatialItemActor : public AActor
{
	GENERATED_BODY()

public:
	AVMeshSpatialItemActor();

	void InitializeFromItem(const FVMRepMeshSpatialItem& InItem);
	void UpdateFromItem(const FVMRepMeshSpatialItem& InItem);

	UStaticMeshComponent* GetMeshComponent() const;
	const FSpatialItemId& GetSpatialItemId() const;
	const FMeshAssetId& GetMeshAssetId() const;
	const TArray<uint32>& GetMaterialIdsBySlot() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	static FTransform ToTransform(const FVMTransformNet& InTransform);
	void ApplyTransform();

	UFUNCTION()
	void OnRep_SpatialTransform();

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(Replicated)
	FSpatialItemId SpatialItemId;

	UPROPERTY(ReplicatedUsing = OnRep_SpatialTransform)
	FVMTransformNet ReplicatedTransform;

	UPROPERTY(Replicated)
	FMeshAssetId MeshAssetId;

	UPROPERTY(Replicated)
	TArray<uint32> MaterialIdsBySlot;

	UPROPERTY(Replicated)
	bool bHasFlags = false;

	UPROPERTY(Replicated)
	uint8 Flags = 0;
};
