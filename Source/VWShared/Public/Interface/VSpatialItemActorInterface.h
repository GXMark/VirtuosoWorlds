#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMRepSpatialItemNet.h"
#include "UObject/Interface.h"
#include "VSpatialItemActorInterface.generated.h"

class UStaticMeshComponent;

UINTERFACE()
class VWSHARED_API USpatialItemActorInterface : public UInterface
{
	GENERATED_BODY()
};

class VWSHARED_API ISpatialItemActorInterface
{
	GENERATED_BODY()

public:
	virtual const FSpatialItemId& GetSpatialItemId() const = 0;
};

UINTERFACE()
class VWSHARED_API USpatialMeshItemActorInterface : public UInterface
{
	GENERATED_BODY()
};

class VWSHARED_API ISpatialMeshItemActorInterface
{
	GENERATED_BODY()

public:
	virtual const FMeshAssetId& GetSpatialMeshAssetId() const = 0;
	virtual const TArray<uint32>& GetSpatialMaterialIdsBySlot() const = 0;
	virtual UStaticMeshComponent* GetSpatialMeshComponent() const = 0;
};
