#include "Replication/VMeshSpatialItemActor.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Utility/VSpatialActorEvents.h"

AVMeshSpatialItemActor::AVMeshSpatialItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(false);
	bAlwaysRelevant = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetMobility(EComponentMobility::Movable);
	SetRootComponent(MeshComp);
}

void AVMeshSpatialItemActor::InitializeFromItem(const FVMRepMeshSpatialItem& InItem)
{
	SpatialItemId = InItem.Id;
	ReplicatedTransform = InItem.Transform;
	MeshAssetId = InItem.MeshAssetId;
	MaterialIdsBySlot.Reset();
	MaterialIdsBySlot.Reserve(InItem.MaterialIdsBySlot.Num());
	for (const FMaterialId& MaterialId : InItem.MaterialIdsBySlot)
	{
		MaterialIdsBySlot.Add(MaterialId.Value);
	}
	bHasFlags = InItem.bHasFlags;
	Flags = InItem.Flags;

	ApplyTransform();
}

void AVMeshSpatialItemActor::UpdateFromItem(const FVMRepMeshSpatialItem& InItem)
{
	InitializeFromItem(InItem);
}

UStaticMeshComponent* AVMeshSpatialItemActor::GetMeshComponent() const
{
	return MeshComp;
}

const FSpatialItemId& AVMeshSpatialItemActor::GetSpatialItemId() const
{
	return SpatialItemId;
}

const FMeshAssetId& AVMeshSpatialItemActor::GetMeshAssetId() const
{
	return MeshAssetId;
}

const TArray<uint32>& AVMeshSpatialItemActor::GetMaterialIdsBySlot() const
{
	return MaterialIdsBySlot;
}

void AVMeshSpatialItemActor::PostNetInit()
{
	Super::PostNetInit();

	if (GetNetMode() != NM_Client)
	{
		return;
	}

	FSpatialActorEvents::OnSpatialActorPostInit().Broadcast(this, SpatialItemId);
}

const FMeshAssetId& AVMeshSpatialItemActor::GetSpatialMeshAssetId() const
{
	return MeshAssetId;
}

const TArray<uint32>& AVMeshSpatialItemActor::GetSpatialMaterialIdsBySlot() const
{
	return MaterialIdsBySlot;
}

UStaticMeshComponent* AVMeshSpatialItemActor::GetSpatialMeshComponent() const
{
	return MeshComp;
}

void AVMeshSpatialItemActor::OnRep_SpatialTransform()
{
	ApplyTransform();
}

void AVMeshSpatialItemActor::OnRep_MeshAssetId()
{
	FSpatialActorEvents::OnSpatialActorMeshAssetIdChanged().Broadcast(this, MeshAssetId);
}

void AVMeshSpatialItemActor::OnRep_MaterialIdsBySlot()
{
	FSpatialActorEvents::OnSpatialActorMaterialIdsChanged().Broadcast(this, MaterialIdsBySlot);
}

void AVMeshSpatialItemActor::ApplyTransform()
{
	SetActorTransform(ToTransform(ReplicatedTransform));
}

FTransform AVMeshSpatialItemActor::ToTransform(const FVMTransformNet& InTransform)
{
	FTransform Out;
	Out.SetLocation(FVector(InTransform.Location));
	Out.SetRotation(InTransform.Rotation.ToQuat());
	Out.SetScale3D(InTransform.bHasScale ? FVector(InTransform.Scale) : FVector(1.f, 1.f, 1.f));
	return Out;
}

void AVMeshSpatialItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVMeshSpatialItemActor, SpatialItemId);
	DOREPLIFETIME(AVMeshSpatialItemActor, ReplicatedTransform);
	DOREPLIFETIME(AVMeshSpatialItemActor, MeshAssetId);
	DOREPLIFETIME(AVMeshSpatialItemActor, MaterialIdsBySlot);
	DOREPLIFETIME(AVMeshSpatialItemActor, bHasFlags);
	DOREPLIFETIME(AVMeshSpatialItemActor, Flags);
}
