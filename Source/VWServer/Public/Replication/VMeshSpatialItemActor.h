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

	UPROPERTY(VisibleAnywhere, Category = "Spatial Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(Replicated, Category = "Spatial Item")
	FSpatialItemId SpatialItemId;

	UPROPERTY(ReplicatedUsing = OnRep_SpatialTransform, Category = "Spatial Item")
	FVMTransformNet ReplicatedTransform;

	UPROPERTY(Replicated, Category = "Spatial Item")
	FMeshAssetId MeshAssetId;

	UPROPERTY(Replicated, Category = "Spatial Item")
	TArray<uint32> MaterialIdsBySlot;

	UPROPERTY(Replicated, Category = "Spatial Item")
	bool bHasFlags = false;

	UPROPERTY(Replicated, Category = "Spatial Item")
	uint8 Flags = 0;
};
