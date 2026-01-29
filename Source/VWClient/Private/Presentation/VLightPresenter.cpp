#include "Presentation/VLightPresenter.h"

#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"

namespace
{
	ELightUnits ResolveLightUnits(const FName& Units)
	{
		if (Units == TEXT("Candelas"))
		{
			return ELightUnits::Candelas;
		}
		if (Units == TEXT("Lumens"))
		{
			return ELightUnits::Lumens;
		}
		return ELightUnits::Unitless;
	}
}

void UVLightPresenter::Initialize(AActor* InOwner, USceneComponent* InPresentationRoot)
{
	PresentationOwner = InOwner;
	PresentationRoot = InPresentationRoot;
}

void UVLightPresenter::PresentPointLightItem(
	const FGuid& InItemId,
	const FVMPointLightComponentNet& InLightData,
	const FTransform& InWorldTransform)
{
	UPointLightComponent* LightComp = GetOrCreatePointLight(InItemId);
	if (!LightComp)
	{
		return;
	}

	LightComp->SetWorldTransform(InWorldTransform);
	ApplyPointLightPayload(LightComp, InLightData);
}

void UVLightPresenter::PresentSpotLightItem(
	const FGuid& InItemId,
	const FVMSpotLightComponentNet& InLightData,
	const FTransform& InWorldTransform)
{
	USpotLightComponent* LightComp = GetOrCreateSpotLight(InItemId);
	if (!LightComp)
	{
		return;
	}

	LightComp->SetWorldTransform(InWorldTransform);
	ApplySpotLightPayload(LightComp, InLightData);
}

void UVLightPresenter::DestroyItem(const FGuid& InItemId)
{
	if (USceneComponent* Comp = SpawnedComponents.FindRef(InItemId))
	{
		Comp->DestroyComponent();
	}
	SpawnedComponents.Remove(InItemId);
}

UPointLightComponent* UVLightPresenter::GetOrCreatePointLight(const FGuid& InItemId)
{
	if (!PresentationOwner.IsValid() || !PresentationRoot.IsValid())
	{
		return nullptr;
	}

	if (USceneComponent* Existing = SpawnedComponents.FindRef(InItemId))
	{
		if (UPointLightComponent* LightComp = Cast<UPointLightComponent>(Existing))
		{
			return LightComp;
		}
		Existing->DestroyComponent();
		SpawnedComponents.Remove(InItemId);
	}

	UPointLightComponent* LightComp = NewObject<UPointLightComponent>(PresentationOwner.Get());
	LightComp->AttachToComponent(PresentationRoot.Get(), FAttachmentTransformRules::KeepWorldTransform);
	LightComp->RegisterComponent();
	SpawnedComponents.Add(InItemId, LightComp);
	return LightComp;
}

USpotLightComponent* UVLightPresenter::GetOrCreateSpotLight(const FGuid& InItemId)
{
	if (!PresentationOwner.IsValid() || !PresentationRoot.IsValid())
	{
		return nullptr;
	}

	if (USceneComponent* Existing = SpawnedComponents.FindRef(InItemId))
	{
		if (USpotLightComponent* LightComp = Cast<USpotLightComponent>(Existing))
		{
			return LightComp;
		}
		Existing->DestroyComponent();
		SpawnedComponents.Remove(InItemId);
	}

	USpotLightComponent* LightComp = NewObject<USpotLightComponent>(PresentationOwner.Get());
	LightComp->AttachToComponent(PresentationRoot.Get(), FAttachmentTransformRules::KeepWorldTransform);
	LightComp->RegisterComponent();
	SpawnedComponents.Add(InItemId, LightComp);
	return LightComp;
}

void UVLightPresenter::ApplyPointLightPayload(UPointLightComponent* LightComp, const FVMPointLightComponentNet& InLightData)
{
	if (!LightComp)
	{
		return;
	}

	LightComp->Intensity = InLightData.intensity;
	LightComp->SetIntensityUnits(ResolveLightUnits(InLightData.units));
	LightComp->SetLightColor(InLightData.color);
	LightComp->SetAttenuationRadius(InLightData.attenuation_radius);
	LightComp->SetSourceRadius(InLightData.source_radius);
	LightComp->SetSoftSourceRadius(InLightData.soft_source_radius);
	LightComp->SetSourceLength(InLightData.source_length);
	LightComp->SetUseTemperature(InLightData.use_temperature);
	LightComp->SetTemperature(InLightData.temperature);
	LightComp->bAffectsWorld = InLightData.affects_world;
	LightComp->CastShadows = InLightData.cast_shadows ? 1 : 0;
	LightComp->SetIndirectLightingIntensity(InLightData.indirect_light_intensity);
	LightComp->SetVolumetricScatteringIntensity(InLightData.volumetric_scatter_intensity);
	LightComp->MarkRenderStateDirty();
}

void UVLightPresenter::ApplySpotLightPayload(USpotLightComponent* LightComp, const FVMSpotLightComponentNet& InLightData)
{
	if (!LightComp)
	{
		return;
	}

	LightComp->Intensity = InLightData.intensity;
	LightComp->SetIntensityUnits(ResolveLightUnits(InLightData.units));
	LightComp->SetLightColor(InLightData.color);
	LightComp->SetAttenuationRadius(InLightData.attenuation_radius);
	LightComp->SetInnerConeAngle(InLightData.inner_cone_angle);
	LightComp->SetOuterConeAngle(InLightData.outer_cone_angle);
	LightComp->SetSourceRadius(InLightData.source_radius);
	LightComp->SetSoftSourceRadius(InLightData.soft_source_radius);
	LightComp->SetSourceLength(InLightData.source_length);
	LightComp->SetUseTemperature(InLightData.use_temperature);
	LightComp->SetTemperature(InLightData.temperature);
	LightComp->bAffectsWorld = InLightData.affects_world;
	LightComp->CastShadows = InLightData.cast_shadows ? 1 : 0;
	LightComp->SetIndirectLightingIntensity(InLightData.indirect_light_intensity);
	LightComp->SetVolumetricScatteringIntensity(InLightData.volumetric_scatter_intensity);
	LightComp->MarkRenderStateDirty();
}
