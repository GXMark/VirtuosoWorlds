#include "Replication/VLightSpatialItemActor.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"
#include "Utility/VSpatialActorEvents.h"

AVLightSpatialItemActor::AVLightSpatialItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(false);
	bAlwaysRelevant = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComp"));
	PointLightComp->SetupAttachment(RootComp);
	PointLightComp->SetMobility(EComponentMobility::Movable);

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	SpotLightComp->SetupAttachment(RootComp);
	SpotLightComp->SetMobility(EComponentMobility::Movable);

	DirectionalLightComp = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLightComp"));
	DirectionalLightComp->SetupAttachment(RootComp);
	DirectionalLightComp->SetMobility(EComponentMobility::Movable);
}

void AVLightSpatialItemActor::InitializeFromItem(const FVMRepLightSpatialItem& InItem)
{
	LightState = InItem;
	ApplyReplicatedLightState();
}

void AVLightSpatialItemActor::UpdateFromItem(const FVMRepLightSpatialItem& InItem)
{
	InitializeFromItem(InItem);
}

const FVMRepLightSpatialItem& AVLightSpatialItemActor::GetLightState() const
{
	return LightState;
}

UDirectionalLightComponent* AVLightSpatialItemActor::GetDirectionalLightComponent() const
{
	return DirectionalLightComp;
}

UPointLightComponent* AVLightSpatialItemActor::GetPointLightComponent() const
{
	return PointLightComp;
}

USpotLightComponent* AVLightSpatialItemActor::GetSpotLightComponent() const
{
	return SpotLightComp;
}

void AVLightSpatialItemActor::PostNetInit()
{
	Super::PostNetInit();

	if (GetNetMode() != NM_Client)
	{
		return;
	}

	ApplyReplicatedLightState();
	FSpatialActorEvents::OnSpatialActorPostInit().Broadcast(this, LightState.Id);
}

const FSpatialItemId& AVLightSpatialItemActor::GetSpatialItemId() const
{
	return LightState.Id;
}

void AVLightSpatialItemActor::OnRep_LightState()
{
	ApplyReplicatedLightState();
}

void AVLightSpatialItemActor::ApplyReplicatedLightState()
{
	SetActorTransform(ToTransform(LightState.Transform));
	UpdateActiveLightComponent();
}

void AVLightSpatialItemActor::ApplyCommonLightSettings(ULightComponent* LightComp) const
{
	if (!LightComp)
		return;

	LightComp->SetIntensity(LightState.Intensity);
	LightComp->SetLightColor(LightState.Color);
	LightComp->SetTemperature(LightState.Temperature);
	LightComp->SetUseTemperature(true);
	LightComp->SetCastShadows(LightState.bCastShadows);
	LightComp->bCastVolumetricShadow = LightState.bCastVolumetricShadow;
	LightComp->VolumetricScatteringIntensity = LightState.bVolumetric ? 1.0f : 0.0f;
	ApplyLightingChannels(LightComp);
	LightComp->MarkRenderStateDirty();
}

void AVLightSpatialItemActor::ApplyLightingChannels(ULightComponent* LightComp) const
{
	if (!LightComp)
		return;

	LightComp->SetLightingChannels(
		(LightState.LightingChannels & 0x1) != 0,
		(LightState.LightingChannels & 0x2) != 0,
		(LightState.LightingChannels & 0x4) != 0);
}

void AVLightSpatialItemActor::UpdateActiveLightComponent() const
{
	PointLightComp->SetVisibility(LightState.Type == EVMRepLightType::Point, true);
	SpotLightComp->SetVisibility(LightState.Type == EVMRepLightType::Spot, true);
	DirectionalLightComp->SetVisibility(LightState.Type == EVMRepLightType::Directional, true);

	switch (LightState.Type)
	{
	case EVMRepLightType::Point:
		PointLightComp->SetAttenuationRadius(LightState.AttenuationRadius);
		ApplyCommonLightSettings(PointLightComp);
		break;
	case EVMRepLightType::Spot:
		SpotLightComp->SetAttenuationRadius(LightState.AttenuationRadius);
		SpotLightComp->SetInnerConeAngle(LightState.InnerConeAngle);
		SpotLightComp->SetOuterConeAngle(LightState.OuterConeAngle);
		ApplyCommonLightSettings(SpotLightComp);
		break;
	case EVMRepLightType::Directional:
		ApplyCommonLightSettings(DirectionalLightComp);
		break;
	default:
		break;
	}
}

FTransform AVLightSpatialItemActor::ToTransform(const FVMTransformNet& InTransform)
{
	FTransform Out;
	Out.SetLocation(FVector(InTransform.Location));
	Out.SetRotation(InTransform.Rotation.ToQuat());
	Out.SetScale3D(InTransform.bHasScale ? FVector(InTransform.Scale) : FVector(1.f, 1.f, 1.f));
	return Out;
}

void AVLightSpatialItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVLightSpatialItemActor, LightState);
}
