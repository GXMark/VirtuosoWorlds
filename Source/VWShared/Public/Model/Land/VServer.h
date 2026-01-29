#pragma once

#include "CoreMinimal.h"
#include "VServer.generated.h"

USTRUCT()
struct VWSHARED_API FVServer
{
	GENERATED_BODY()

	UPROPERTY()
	bool land_registry; // determines if the server/region/parcels are registered with the land registry
	
	UPROPERTY()
	bool empty_cache; // remove all assets from the disk cache

	UPROPERTY()
	bool empty_db; // remove all items from the database

	UPROPERTY()
	bool package_imports; // reads the package folder at start up

	UPROPERTY()
	FString package_path;	// Absolute path to the package folder
	
	UPROPERTY()
	FString cache_path;	// Absolute path to the cache folder of assets

	UPROPERTY()
	FString cdn_url; // Content delivery network (CDN) URL

	UPROPERTY()
	FString cdn_password; // Content delivery network (CDN) plain text password

	UPROPERTY()
	float spawn_radius;  // Maximum radius of spawned actor positions before player

	FVServer(): land_registry(false), empty_cache(false), empty_db(false), package_imports(false), cache_path(TEXT("")), cdn_url(TEXT("")), cdn_password(TEXT("")), spawn_radius(0)
	{
	}
};
