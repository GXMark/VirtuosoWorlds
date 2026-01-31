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
	bIsStaticMeshItem = IsStaticItem(InItem);

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
	ApplyDormancyFromItem(InItem, false);
}

void AVMeshSpatialItemActor::UpdateFromItem(const FVMRepMeshSpatialItem& InItem)
{
	const bool bTransformChanged = HasTransformChanged(ReplicatedTransform, InItem.Transform);
	const bool bMaterialsChanged = HasMaterialIdsChanged(MaterialIdsBySlot, InItem.MaterialIdsBySlot);

	InitializeFromItem(InItem);
	ApplyDormancyFromItem(InItem, bTransformChanged || bMaterialsChanged);
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

bool AVMeshSpatialItemActor::IsStaticItem(const FVMRepMeshSpatialItem& InItem)
{
	constexpr uint8 StaticFlag = 1 << 0;
	return InItem.bHasFlags && (InItem.Flags & StaticFlag) != 0;
}

bool AVMeshSpatialItemActor::HasTransformChanged(const FVMTransformNet& Current, const FVMTransformNet& Incoming)
{
	if (Current.Location != Incoming.Location)
	{
		return true;
	}

	if (!Current.Rotation.Identical(&Incoming.Rotation, 0))
	{
		return true;
	}

	if (Current.bHasScale != Incoming.bHasScale)
	{
		return true;
	}

	return Current.bHasScale && Current.Scale != Incoming.Scale;
}

bool AVMeshSpatialItemActor::HasMaterialIdsChanged(
	const TArray<uint32>& Current,
	const TArray<FMaterialId>& Incoming)
{
	if (Current.Num() != Incoming.Num())
	{
		return true;
	}

	for (int32 Index = 0; Index < Current.Num(); ++Index)
	{
		if (Current[Index] != Incoming[Index].Value)
		{
			return true;
		}
	}

	return false;
}

FTransform AVMeshSpatialItemActor::ToTransform(const FVMTransformNet& InTransform)
{
	FTransform Out;
	Out.SetLocation(FVector(InTransform.Location));
	Out.SetRotation(InTransform.Rotation.ToQuat());
	Out.SetScale3D(InTransform.bHasScale ? FVector(InTransform.Scale) : FVector(1.f, 1.f, 1.f));
	return Out;
}

void AVMeshSpatialItemActor::ApplyDormancyFromItem(const FVMRepMeshSpatialItem& InItem, bool bForceWake)
{
	if (!HasAuthority() || !bIsStaticMeshItem)
	{
		return;
	}

	if (bForceWake)
	{
		SetNetDormancy(DORM_Awake);
		ForceNetUpdate();
		SetNetDormancy(DORM_DormantAll);
		return;
	}

	if (InItem.bHasFlags)
	{
		SetNetDormancy(DORM_Initial);
	}
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
