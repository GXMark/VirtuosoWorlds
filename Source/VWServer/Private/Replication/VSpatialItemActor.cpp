#include "Replication/VSpatialItemActor.h"

#include "Engine/NetConnection.h"
#include "Net/UnrealNetwork.h"
#include "Replication/VReplicationManager.h"

AVSpatialItemActor::AVSpatialItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(false);
	bAlwaysRelevant = false;
}

void AVSpatialItemActor::InitializeSpatialItem(const FVMSpatialItemNet& InItem)
{
	SpatialItem = InItem;
}

void AVSpatialItemActor::UpdateSpatialItem(const FVMSpatialItemNet& InItem)
{
	SpatialItem = InItem;
}

const FVMSpatialItemNet& AVSpatialItemActor::GetSpatialItem() const
{
	return SpatialItem;
}

bool AVSpatialItemActor::IsNetRelevantFor(
	const AActor* RealViewer,
	const AActor* ViewTarget,
	const FVector& SrcLocation) const
{
	const UWorld* World = GetWorld();
	const UVReplicationManager* ReplicationManager = World ? World->GetSubsystem<UVReplicationManager>() : nullptr;
	const UNetConnection* Connection = RealViewer ? RealViewer->GetNetConnection() : nullptr;

	if (!ReplicationManager || !Connection || !ReplicationManager->HasConnectionRelevancy(Connection))
	{
		return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	}

	return ReplicationManager->IsSpatialItemRelevantForConnection(SpatialItem.ItemID.Value, Connection);
}

void AVSpatialItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVSpatialItemActor, SpatialItem);
}
