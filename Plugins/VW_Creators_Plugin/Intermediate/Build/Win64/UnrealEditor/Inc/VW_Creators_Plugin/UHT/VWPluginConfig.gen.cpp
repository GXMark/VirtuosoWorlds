// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Config/VWPluginConfig.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVWPluginConfig() {}

// ********** Begin Cross Module References ********************************************************
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FVWPluginConfig();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FVWPluginConfig ***************************************************
struct Z_Construct_UScriptStruct_FVWPluginConfig_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FVWPluginConfig); }
	static inline consteval int16 GetStructAlignment() { return alignof(FVWPluginConfig); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ClearLogs_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShowLogs_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShowJson_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ClearAllAssets_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DontPackageAssets_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PackageName_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PackageDescription_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PackageAuthor_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PackagePath_MetaData[] = {
		{ "Category", "VWPluginConfig" },
		{ "ModuleRelativePath", "Public/Model/Config/VWPluginConfig.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FVWPluginConfig constinit property declarations *******************
	static void NewProp_ClearLogs_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ClearLogs;
	static void NewProp_ShowLogs_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowLogs;
	static void NewProp_ShowJson_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ShowJson;
	static void NewProp_ClearAllAssets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ClearAllAssets;
	static void NewProp_DontPackageAssets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_DontPackageAssets;
	static const UECodeGen_Private::FStrPropertyParams NewProp_PackageName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_PackageDescription;
	static const UECodeGen_Private::FStrPropertyParams NewProp_PackageAuthor;
	static const UECodeGen_Private::FStrPropertyParams NewProp_PackagePath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FVWPluginConfig constinit property declarations *********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVWPluginConfig>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FVWPluginConfig_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FVWPluginConfig;
class UScriptStruct* FVWPluginConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FVWPluginConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FVWPluginConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVWPluginConfig, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("VWPluginConfig"));
	}
	return Z_Registration_Info_UScriptStruct_FVWPluginConfig.OuterSingleton;
	}

// ********** Begin ScriptStruct FVWPluginConfig Property Definitions ******************************
void Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearLogs_SetBit(void* Obj)
{
	((FVWPluginConfig*)Obj)->ClearLogs = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearLogs = { "ClearLogs", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVWPluginConfig), &Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearLogs_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ClearLogs_MetaData), NewProp_ClearLogs_MetaData) };
void Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowLogs_SetBit(void* Obj)
{
	((FVWPluginConfig*)Obj)->ShowLogs = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowLogs = { "ShowLogs", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVWPluginConfig), &Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowLogs_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShowLogs_MetaData), NewProp_ShowLogs_MetaData) };
void Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowJson_SetBit(void* Obj)
{
	((FVWPluginConfig*)Obj)->ShowJson = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowJson = { "ShowJson", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVWPluginConfig), &Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowJson_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShowJson_MetaData), NewProp_ShowJson_MetaData) };
void Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearAllAssets_SetBit(void* Obj)
{
	((FVWPluginConfig*)Obj)->ClearAllAssets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearAllAssets = { "ClearAllAssets", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVWPluginConfig), &Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearAllAssets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ClearAllAssets_MetaData), NewProp_ClearAllAssets_MetaData) };
void Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_DontPackageAssets_SetBit(void* Obj)
{
	((FVWPluginConfig*)Obj)->DontPackageAssets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_DontPackageAssets = { "DontPackageAssets", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVWPluginConfig), &Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_DontPackageAssets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DontPackageAssets_MetaData), NewProp_DontPackageAssets_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackageName = { "PackageName", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVWPluginConfig, PackageName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PackageName_MetaData), NewProp_PackageName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackageDescription = { "PackageDescription", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVWPluginConfig, PackageDescription), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PackageDescription_MetaData), NewProp_PackageDescription_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackageAuthor = { "PackageAuthor", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVWPluginConfig, PackageAuthor), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PackageAuthor_MetaData), NewProp_PackageAuthor_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackagePath = { "PackagePath", nullptr, (EPropertyFlags)0x0010000000000004, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVWPluginConfig, PackagePath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PackagePath_MetaData), NewProp_PackagePath_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVWPluginConfig_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearLogs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowLogs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ShowJson,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_ClearAllAssets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_DontPackageAssets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackageName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackageDescription,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackageAuthor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewProp_PackagePath,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVWPluginConfig_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FVWPluginConfig Property Definitions ********************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVWPluginConfig_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"VWPluginConfig",
	Z_Construct_UScriptStruct_FVWPluginConfig_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVWPluginConfig_Statics::PropPointers),
	sizeof(FVWPluginConfig),
	alignof(FVWPluginConfig),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVWPluginConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVWPluginConfig_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FVWPluginConfig()
{
	if (!Z_Registration_Info_UScriptStruct_FVWPluginConfig.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FVWPluginConfig.InnerSingleton, Z_Construct_UScriptStruct_FVWPluginConfig_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FVWPluginConfig.InnerSingleton);
}
// ********** End ScriptStruct FVWPluginConfig *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Config_VWPluginConfig_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FVWPluginConfig::StaticStruct, Z_Construct_UScriptStruct_FVWPluginConfig_Statics::NewStructOps, TEXT("VWPluginConfig"),&Z_Registration_Info_UScriptStruct_FVWPluginConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVWPluginConfig), 3977939904U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Config_VWPluginConfig_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Config_VWPluginConfig_h__Script_VW_Creators_Plugin_1283745132{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Config_VWPluginConfig_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Config_VWPluginConfig_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
