// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Model/Config/VWPluginConfig.h"
#include "Model/Package/MPActor.h"
#include "Model/Package/MPAssetRef.h"
#include "Model/Package/MPPackage.h"
#include "Model/Package/MPMaterial.h"
#include "VW_Creators_PluginBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UVW_Creators_PluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	enum class ECollisionType
	{
		None, Simple, Complex
	};


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Cook Current Level", Keywords = "VW_Creators_Plugin cook current level"), Category = "VW_Creators_Plugin")
	static void CookCurrentLevel();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Package Archive", Keywords = "VW_Creators_Plugin create package archive"), Category = "VW_Creators_Plugin")
	static void CreatePackageArchive(const TArray<AActor*>& SelectedActors);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Cancel Package Build", Keywords = "VW_Creators_Plugin cancel package build"), Category = "VW_Creators_Plugin")
	static void CancelPackageBuild();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Save Plugin Config", Keywords = "VW_Creators_Plugin save plugin config"), Category = "VW_Creators_Plugin")
	static void SaveConfig(const bool InClearLog, const bool InShowLogs, const bool InShowJson, const bool InClearAllAssets, const bool InDontPackageAssets, const FString& InPackageName, const FString& InPackageDescription, const FString& InPackageAuthor, const FString& InPackagePath);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load Plugin Config", Keywords = "VW_Creators_Plugin load plugin config"), Category = "VW_Creators_Plugin")
	static bool LoadConfig(UPARAM(ref) FVWPluginConfig& PluginConfig);

private:
	static FMPPackage Package;
	static FString PluginINI;

	static TArray<AActor*> Actors;
	
	static TMap<int32, FGuid> ActorIDMap;		// Maps internal actor ids to guids

	static bool CancelBuild;
	static FString AssetPath;
	static bool ClearLogs;
	static bool ShowLogs;
	static bool ShowJson;
	static bool ClearAllAssets;
	static bool DontPackageAssets;
	static FString PackageName;
	static FString PackageDescription;
	static FString PackageAuthor;
	static FString PackagePath;


	static bool ValidateMaterials(const TArray<AActor*>& SelectedActors);
	static bool ValidateMesh(const TArray<AActor*>& SelectedActors);
	static ECollisionType DetectCollisionType(const UStaticMesh* Mesh);
	static void BuildPackage(AActor* InActor);
	static void FlattenBlueprintActor(const AActor* InRootActor, TArray<FMPActor>& OutActorItems);
	static void BuildPackageArchive();
	static void SerializeActorHierarchy(const AActor* InActor, TArray<FMPActor>& OutActors, TArray<FMPMaterial>& OutMaterialItems);
	static bool SerializeActorComponent(const AActor* InActor, FMPActor& OutActorItem, TArray<FMPMaterial>& OutMaterialItems);
	static void SerializeAsset(const FMPAssetRef& InAssetRef);
	static FGuid GetActorID(int32 InID);
};
