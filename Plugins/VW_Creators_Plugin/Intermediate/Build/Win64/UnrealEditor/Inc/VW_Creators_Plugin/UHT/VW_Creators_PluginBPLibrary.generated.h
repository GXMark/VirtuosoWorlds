// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VW_Creators_PluginBPLibrary.h"

#ifdef VW_CREATORS_PLUGIN_VW_Creators_PluginBPLibrary_generated_h
#error "VW_Creators_PluginBPLibrary.generated.h already included, missing '#pragma once' in VW_Creators_PluginBPLibrary.h"
#endif
#define VW_CREATORS_PLUGIN_VW_Creators_PluginBPLibrary_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
struct FVWPluginConfig;

// ********** Begin Class UVW_Creators_PluginBPLibrary *********************************************
#define FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_RPC_WRAPPERS \
	DECLARE_FUNCTION(execLoadConfig); \
	DECLARE_FUNCTION(execSaveConfig); \
	DECLARE_FUNCTION(execCancelPackageBuild); \
	DECLARE_FUNCTION(execCreatePackageArchive); \
	DECLARE_FUNCTION(execCookCurrentLevel);


struct Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics;
VW_CREATORS_PLUGIN_API UClass* Z_Construct_UClass_UVW_Creators_PluginBPLibrary_NoRegister();

#define FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_INCLASS \
private: \
	static void StaticRegisterNativesUVW_Creators_PluginBPLibrary(); \
	friend struct ::Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VW_CREATORS_PLUGIN_API UClass* ::Z_Construct_UClass_UVW_Creators_PluginBPLibrary_NoRegister(); \
public: \
	DECLARE_CLASS2(UVW_Creators_PluginBPLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/VW_Creators_Plugin"), Z_Construct_UClass_UVW_Creators_PluginBPLibrary_NoRegister) \
	DECLARE_SERIALIZER(UVW_Creators_PluginBPLibrary)


#define FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UVW_Creators_PluginBPLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UVW_Creators_PluginBPLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVW_Creators_PluginBPLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVW_Creators_PluginBPLibrary); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UVW_Creators_PluginBPLibrary(UVW_Creators_PluginBPLibrary&&) = delete; \
	UVW_Creators_PluginBPLibrary(const UVW_Creators_PluginBPLibrary&) = delete; \
	NO_API virtual ~UVW_Creators_PluginBPLibrary();


#define FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_30_PROLOG
#define FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_RPC_WRAPPERS \
	FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_INCLASS \
	FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h_33_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UVW_Creators_PluginBPLibrary;

// ********** End Class UVW_Creators_PluginBPLibrary ***********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
