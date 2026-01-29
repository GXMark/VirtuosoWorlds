#pragma once

#include "CoreMinimal.h"

struct FPTargetType
{
	inline static const FName None = TEXT("None");
	inline static const FName SceneActor = TEXT("SceneActor");
	inline static const FName StaticMeshActor = TEXT("StaticMeshActor");
	inline static const FName HISMActor = TEXT("HISMActor");	
	inline static const FName SkeletalMeshActor = TEXT("SkeletalMeshActor");
	inline static const FName DecalActor = TEXT("DecalActor");
	inline static const FName PointLightActor = TEXT("PointLightActor");
	inline static const FName SpotLightActor = TEXT("SpotLightActor");
	inline static const FName Material = TEXT("Material");
};

struct FPAssetType
{
	inline static const FName None = TEXT("None");
	inline static const FName Package = TEXT("Package");
	inline static const FName Material = TEXT("Material");
	inline static const FName Texture = TEXT("Texture");
	inline static const FName Mesh = TEXT("Mesh");
	inline static const FName Decal = TEXT("Decal");
	inline static const FName SpotLight = TEXT("SpotLight");
	inline static const FName PointLight = TEXT("PointLight");
};

struct FPLightUnitType
{
	inline static const FName Unitless = TEXT("Unitless");
	inline static const FName Candelas = TEXT("Candelas");
	inline static const FName Lumens = TEXT("Lumens");
	inline static const FName EV = TEXT("EV");
	inline static const FName Nits = TEXT("Nits");
};

