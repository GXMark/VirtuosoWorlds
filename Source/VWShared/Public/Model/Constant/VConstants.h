#pragma once

#include "CoreMinimal.h"

struct FVServerType
{
	inline static const FName None = TEXT("None");
	inline static const FName Game = TEXT("Game");
	inline static const FName Editor = TEXT("Editor");
	inline static const FName CDN = TEXT("CDN");
};

struct FVActorType
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

struct FVAssetType
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

struct FVCacheType
{
	inline static const FName NotFound = TEXT("NotFound");	// Cache key valid but not found
	inline static const FName Invalid = TEXT("Invalid");	// Cache key not valid
	inline static const FName Empty = TEXT("Empty");		// Cache key is empty
	inline static const FName Local = TEXT("Local");		// Cached locally either in memory or on disk
	inline static const FName Memory = TEXT("Memory");		// Cached locally in memory
	inline static const FName Disk = TEXT("Disk");			// Cached on locally on disk
	inline static const FName Remote = TEXT("Remote");		// Cached remotely
};

struct FVLightUnitType
{
	inline static const FName Unitless = TEXT("Unitless");
	inline static const FName Candelas = TEXT("Candelas");
	inline static const FName Lumens = TEXT("Lumens");
	inline static const FName EV = TEXT("EV");
	inline static const FName Nits = TEXT("Nits");
};