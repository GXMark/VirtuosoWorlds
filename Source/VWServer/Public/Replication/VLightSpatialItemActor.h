#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "VLightSpatialItemActor.generated.h"

class UDirectionalLightComponent;
class ULightComponentBase;
class UPointLightComponent;
class USceneComponent;
class USpotLightComponent;

UCLASS()
class VWSERVER_API AVLightSpatialItemActor : public AActor
{
	GENERATED_BODY()

public:
	AVLightSpatialItemActor();

	void InitializeFromItem(const FVMRepLightSpatialItem& InItem);
	void UpdateFromItem(const FVMRepLightSpatialItem& InItem);
	const FVMRepLightSpatialItem& GetLightState() const;

	UDirectionalLightComponent* GetDirectionalLightComponent() const;
	UPointLightComponent* GetPointLightComponent() const;
	USpotLightComponent* GetSpotLightComponent() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	static FTransform ToTransform(const FVMTransformNet& InTransform);
	void ApplyLightState();
	void ApplyCommonLightSettings(ULightComponent* LightComp) const;
	void ApplyLightingChannels(ULightComponent* LightComp) const;
	void UpdateActiveLightComponent() const;

	UFUNCTION()
	void OnRep_LightState();

	UPROPERTY()
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> PointLightComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpotLightComponent> SpotLightComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDirectionalLightComponent> DirectionalLightComp;

	UPROPERTY(ReplicatedUsing = OnRep_LightState)
	FVMRepLightSpatialItem LightState;
};
