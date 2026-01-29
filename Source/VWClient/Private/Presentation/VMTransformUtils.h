#pragma once

#include "CoreMinimal.h"
#include "Model/Network/VMSpatialItemNet.h"

FORCEINLINE FTransform VMToTransform(const FVMTransformNet& Net)
{
	FTransform Xf;
	Xf.SetLocation(FVector(Net.Location));
	Xf.SetRotation(Net.Rotation.ToQuat());
	Xf.SetScale3D(Net.bHasScale ? FVector(Net.Scale) : FVector(1.f, 1.f, 1.f));
	return Xf;
}
