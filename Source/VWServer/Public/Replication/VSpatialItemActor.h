#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/VMSpatialItemNet.h"
#include "Replication/VReplicationManager.h"
#include "VSpatialItemActor.generated.h"

UCLASS()
class VWSERVER_API AVSpatialItemActor : public AActor
{
	GENERATED_BODY()

public:
	AVSpatialItemActor();

	void InitializeSpatialItem(const FVMSpatialItemNet& InItem);
	void UpdateSpatialItem(const FVMSpatialItemNet& InItem);
	const FVMSpatialItemNet& GetSpatialItem() const;

	virtual bool IsNetRelevantFor(
		const AActor* RealViewer,
		const AActor* ViewTarget,
		const FVector& SrcLocation) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FVMSpatialItemNet SpatialItem;
};
