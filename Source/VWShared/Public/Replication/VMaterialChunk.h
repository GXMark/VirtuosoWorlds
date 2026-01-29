#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model/Network/Chunk/VReplicatedMaterialFastArray.h"
#include "VMaterialChunk.generated.h"

/**
 * Cold-lane material chunk replicated via FastArray.
 * Presentation/application to UMaterialInstanceDynamic can be layered later.
 */
UCLASS()
class VWSHARED_API AVMaterialChunk : public AActor
{
	GENERATED_BODY()

public:
	AVMaterialChunk();

	UPROPERTY(Replicated)
	FVReplicatedMaterialItemArray MaterialItems;

	// Server API
	void ServerAddOrUpdateMaterial(const FVMMaterialNet& InMaterial);
	void ServerRemoveMaterial(const FGuid& InMaterialID);

	// Called by FastArray callbacks
	void HandleMaterialAdded(const FVMMaterialNet& InMaterial);
	void HandleMaterialChanged(const FVMMaterialNet& InMaterial);
	void HandleMaterialRemoved(const FGuid& InMaterialID);

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	int32 FindIndexByMaterialID(const FGuid& InMaterialID) const;
};
