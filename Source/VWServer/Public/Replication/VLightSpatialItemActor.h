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
	void ApplyCommonLightSettings(ULightComponentBase* LightComp) const;
	void ApplyLightingChannels(ULightComponentBase* LightComp) const;
	void UpdateActiveLightComponent();

	UFUNCTION()
	void OnRep_LightState();

	UPROPERTY(VisibleAnywhere, Category = "Spatial Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Spatial Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPointLightComponent> PointLightComp;

	UPROPERTY(VisibleAnywhere, Category = "Spatial Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpotLightComponent> SpotLightComp;

	UPROPERTY(VisibleAnywhere, Category = "Spatial Item", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDirectionalLightComponent> DirectionalLightComp;

	UPROPERTY(ReplicatedUsing = OnRep_LightState, Category = "Spatial Item")
	FVMRepLightSpatialItem LightState;
};
