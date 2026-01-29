// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VW_Creators_PluginBPLibrary.h"
#include "Model/Config/VWPluginConfig.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVW_Creators_PluginBPLibrary() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UClass* Z_Construct_UClass_UVW_Creators_PluginBPLibrary();
VW_CREATORS_PLUGIN_API UClass* Z_Construct_UClass_UVW_Creators_PluginBPLibrary_NoRegister();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FVWPluginConfig();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVW_Creators_PluginBPLibrary Function CancelPackageBuild *****************
struct Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "VW_Creators_Plugin" },
		{ "DisplayName", "Cancel Package Build" },
		{ "Keywords", "VW_Creators_Plugin cancel package build" },
		{ "ModuleRelativePath", "Public/VW_Creators_PluginBPLibrary.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CancelPackageBuild constinit property declarations ********************
// ********** End Function CancelPackageBuild constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVW_Creators_PluginBPLibrary, nullptr, "CancelPackageBuild", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVW_Creators_PluginBPLibrary::execCancelPackageBuild)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	UVW_Creators_PluginBPLibrary::CancelPackageBuild();
	P_NATIVE_END;
}
// ********** End Class UVW_Creators_PluginBPLibrary Function CancelPackageBuild *******************

// ********** Begin Class UVW_Creators_PluginBPLibrary Function CookCurrentLevel *******************
struct Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "VW_Creators_Plugin" },
		{ "DisplayName", "Cook Current Level" },
		{ "Keywords", "VW_Creators_Plugin cook current level" },
		{ "ModuleRelativePath", "Public/VW_Creators_PluginBPLibrary.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function CookCurrentLevel constinit property declarations **********************
// ********** End Function CookCurrentLevel constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVW_Creators_PluginBPLibrary, nullptr, "CookCurrentLevel", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVW_Creators_PluginBPLibrary::execCookCurrentLevel)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	UVW_Creators_PluginBPLibrary::CookCurrentLevel();
	P_NATIVE_END;
}
// ********** End Class UVW_Creators_PluginBPLibrary Function CookCurrentLevel *********************

// ********** Begin Class UVW_Creators_PluginBPLibrary Function CreatePackageArchive ***************
struct Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics
{
	struct VW_Creators_PluginBPLibrary_eventCreatePackageArchive_Parms
	{
		TArray<AActor*> SelectedActors;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "VW_Creators_Plugin" },
		{ "DisplayName", "Create Package Archive" },
		{ "Keywords", "VW_Creators_Plugin create package archive" },
		{ "ModuleRelativePath", "Public/VW_Creators_PluginBPLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectedActors_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function CreatePackageArchive constinit property declarations ******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SelectedActors_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_SelectedActors;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function CreatePackageArchive constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function CreatePackageArchive Property Definitions *****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::NewProp_SelectedActors_Inner = { "SelectedActors", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::NewProp_SelectedActors = { "SelectedActors", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VW_Creators_PluginBPLibrary_eventCreatePackageArchive_Parms, SelectedActors), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectedActors_MetaData), NewProp_SelectedActors_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::NewProp_SelectedActors_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::NewProp_SelectedActors,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::PropPointers) < 2048);
// ********** End Function CreatePackageArchive Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVW_Creators_PluginBPLibrary, nullptr, "CreatePackageArchive", 	Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::VW_Creators_PluginBPLibrary_eventCreatePackageArchive_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::VW_Creators_PluginBPLibrary_eventCreatePackageArchive_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVW_Creators_PluginBPLibrary::execCreatePackageArchive)
{
	P_GET_TARRAY_REF(AActor*,Z_Param_Out_SelectedActors);
	P_FINISH;
	P_NATIVE_BEGIN;
	UVW_Creators_PluginBPLibrary::CreatePackageArchive(Z_Param_Out_SelectedActors);
	P_NATIVE_END;
}
// ********** End Class UVW_Creators_PluginBPLibrary Function CreatePackageArchive *****************

// ********** Begin Class UVW_Creators_PluginBPLibrary Function LoadConfig *************************
struct Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics
{
	struct VW_Creators_PluginBPLibrary_eventLoadConfig_Parms
	{
		FVWPluginConfig PluginConfig;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "VW_Creators_Plugin" },
		{ "DisplayName", "Load Plugin Config" },
		{ "Keywords", "VW_Creators_Plugin load plugin config" },
		{ "ModuleRelativePath", "Public/VW_Creators_PluginBPLibrary.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function LoadConfig constinit property declarations ****************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_PluginConfig;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function LoadConfig constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function LoadConfig Property Definitions ***************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::NewProp_PluginConfig = { "PluginConfig", nullptr, (EPropertyFlags)0x0010000008000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VW_Creators_PluginBPLibrary_eventLoadConfig_Parms, PluginConfig), Z_Construct_UScriptStruct_FVWPluginConfig, METADATA_PARAMS(0, nullptr) }; // 3977939904
void Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VW_Creators_PluginBPLibrary_eventLoadConfig_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VW_Creators_PluginBPLibrary_eventLoadConfig_Parms), &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::NewProp_PluginConfig,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::PropPointers) < 2048);
// ********** End Function LoadConfig Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVW_Creators_PluginBPLibrary, nullptr, "LoadConfig", 	Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::VW_Creators_PluginBPLibrary_eventLoadConfig_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::VW_Creators_PluginBPLibrary_eventLoadConfig_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVW_Creators_PluginBPLibrary::execLoadConfig)
{
	P_GET_STRUCT_REF(FVWPluginConfig,Z_Param_Out_PluginConfig);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UVW_Creators_PluginBPLibrary::LoadConfig(Z_Param_Out_PluginConfig);
	P_NATIVE_END;
}
// ********** End Class UVW_Creators_PluginBPLibrary Function LoadConfig ***************************

// ********** Begin Class UVW_Creators_PluginBPLibrary Function SaveConfig *************************
struct Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics
{
	struct VW_Creators_PluginBPLibrary_eventSaveConfig_Parms
	{
		bool InClearLog;
		bool InShowLogs;
		bool InShowJson;
		bool InClearAllAssets;
		bool InDontPackageAssets;
		FString InPackageName;
		FString InPackageDescription;
		FString InPackageAuthor;
		FString InPackagePath;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "VW_Creators_Plugin" },
		{ "DisplayName", "Save Plugin Config" },
		{ "Keywords", "VW_Creators_Plugin save plugin config" },
		{ "ModuleRelativePath", "Public/VW_Creators_PluginBPLibrary.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InClearLog_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InShowLogs_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InShowJson_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InClearAllAssets_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InDontPackageAssets_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InPackageName_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InPackageDescription_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InPackageAuthor_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InPackagePath_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SaveConfig constinit property declarations ****************************
	static void NewProp_InClearLog_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_InClearLog;
	static void NewProp_InShowLogs_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_InShowLogs;
	static void NewProp_InShowJson_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_InShowJson;
	static void NewProp_InClearAllAssets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_InClearAllAssets;
	static void NewProp_InDontPackageAssets_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_InDontPackageAssets;
	static const UECodeGen_Private::FStrPropertyParams NewProp_InPackageName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_InPackageDescription;
	static const UECodeGen_Private::FStrPropertyParams NewProp_InPackageAuthor;
	static const UECodeGen_Private::FStrPropertyParams NewProp_InPackagePath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SaveConfig constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SaveConfig Property Definitions ***************************************
void Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearLog_SetBit(void* Obj)
{
	((VW_Creators_PluginBPLibrary_eventSaveConfig_Parms*)Obj)->InClearLog = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearLog = { "InClearLog", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms), &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearLog_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InClearLog_MetaData), NewProp_InClearLog_MetaData) };
void Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowLogs_SetBit(void* Obj)
{
	((VW_Creators_PluginBPLibrary_eventSaveConfig_Parms*)Obj)->InShowLogs = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowLogs = { "InShowLogs", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms), &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowLogs_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InShowLogs_MetaData), NewProp_InShowLogs_MetaData) };
void Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowJson_SetBit(void* Obj)
{
	((VW_Creators_PluginBPLibrary_eventSaveConfig_Parms*)Obj)->InShowJson = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowJson = { "InShowJson", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms), &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowJson_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InShowJson_MetaData), NewProp_InShowJson_MetaData) };
void Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearAllAssets_SetBit(void* Obj)
{
	((VW_Creators_PluginBPLibrary_eventSaveConfig_Parms*)Obj)->InClearAllAssets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearAllAssets = { "InClearAllAssets", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms), &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearAllAssets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InClearAllAssets_MetaData), NewProp_InClearAllAssets_MetaData) };
void Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InDontPackageAssets_SetBit(void* Obj)
{
	((VW_Creators_PluginBPLibrary_eventSaveConfig_Parms*)Obj)->InDontPackageAssets = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InDontPackageAssets = { "InDontPackageAssets", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms), &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InDontPackageAssets_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InDontPackageAssets_MetaData), NewProp_InDontPackageAssets_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackageName = { "InPackageName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms, InPackageName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InPackageName_MetaData), NewProp_InPackageName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackageDescription = { "InPackageDescription", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms, InPackageDescription), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InPackageDescription_MetaData), NewProp_InPackageDescription_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackageAuthor = { "InPackageAuthor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms, InPackageAuthor), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InPackageAuthor_MetaData), NewProp_InPackageAuthor_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackagePath = { "InPackagePath", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VW_Creators_PluginBPLibrary_eventSaveConfig_Parms, InPackagePath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InPackagePath_MetaData), NewProp_InPackagePath_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearLog,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowLogs,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InShowJson,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InClearAllAssets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InDontPackageAssets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackageName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackageDescription,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackageAuthor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::NewProp_InPackagePath,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::PropPointers) < 2048);
// ********** End Function SaveConfig Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVW_Creators_PluginBPLibrary, nullptr, "SaveConfig", 	Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::VW_Creators_PluginBPLibrary_eventSaveConfig_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::VW_Creators_PluginBPLibrary_eventSaveConfig_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVW_Creators_PluginBPLibrary::execSaveConfig)
{
	P_GET_UBOOL(Z_Param_InClearLog);
	P_GET_UBOOL(Z_Param_InShowLogs);
	P_GET_UBOOL(Z_Param_InShowJson);
	P_GET_UBOOL(Z_Param_InClearAllAssets);
	P_GET_UBOOL(Z_Param_InDontPackageAssets);
	P_GET_PROPERTY(FStrProperty,Z_Param_InPackageName);
	P_GET_PROPERTY(FStrProperty,Z_Param_InPackageDescription);
	P_GET_PROPERTY(FStrProperty,Z_Param_InPackageAuthor);
	P_GET_PROPERTY(FStrProperty,Z_Param_InPackagePath);
	P_FINISH;
	P_NATIVE_BEGIN;
	UVW_Creators_PluginBPLibrary::SaveConfig(Z_Param_InClearLog,Z_Param_InShowLogs,Z_Param_InShowJson,Z_Param_InClearAllAssets,Z_Param_InDontPackageAssets,Z_Param_InPackageName,Z_Param_InPackageDescription,Z_Param_InPackageAuthor,Z_Param_InPackagePath);
	P_NATIVE_END;
}
// ********** End Class UVW_Creators_PluginBPLibrary Function SaveConfig ***************************

// ********** Begin Class UVW_Creators_PluginBPLibrary *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary;
UClass* UVW_Creators_PluginBPLibrary::GetPrivateStaticClass()
{
	using TClass = UVW_Creators_PluginBPLibrary;
	if (!Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VW_Creators_PluginBPLibrary"),
			Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary.InnerSingleton,
			StaticRegisterNativesUVW_Creators_PluginBPLibrary,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary.InnerSingleton;
}
UClass* Z_Construct_UClass_UVW_Creators_PluginBPLibrary_NoRegister()
{
	return UVW_Creators_PluginBPLibrary::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/* \n*\x09""Function library class.\n*\x09""Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.\n*\n*\x09When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.\n*\x09""BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.\n*\x09""BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.\n*\x09""DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.\n*\x09\x09\x09\x09Its lets you name the node using characters not allowed in C++ function names.\n*\x09""CompactNodeTitle - the word(s) that appear on the node.\n*\x09Keywords -\x09the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. \n*\x09\x09\x09\x09Good example is \"Print String\" node which you can find also by using keyword \"log\".\n*\x09""Category -\x09the category your node will be under in the Blueprint drop-down menu.\n*\n*\x09""For more info on custom blueprint nodes visit documentation:\n*\x09https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation\n*/" },
#endif
		{ "IncludePath", "VW_Creators_PluginBPLibrary.h" },
		{ "ModuleRelativePath", "Public/VW_Creators_PluginBPLibrary.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "*      Function library class.\n*      Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.\n*\n*      When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.\n*      BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.\n*      BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.\n*      DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.\n*                              Its lets you name the node using characters not allowed in C++ function names.\n*      CompactNodeTitle - the word(s) that appear on the node.\n*      Keywords -      the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.\n*                              Good example is \"Print String\" node which you can find also by using keyword \"log\".\n*      Category -      the category your node will be under in the Blueprint drop-down menu.\n*\n*      For more info on custom blueprint nodes visit documentation:\n*      https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UVW_Creators_PluginBPLibrary constinit property declarations *************
// ********** End Class UVW_Creators_PluginBPLibrary constinit property declarations ***************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("CancelPackageBuild"), .Pointer = &UVW_Creators_PluginBPLibrary::execCancelPackageBuild },
		{ .NameUTF8 = UTF8TEXT("CookCurrentLevel"), .Pointer = &UVW_Creators_PluginBPLibrary::execCookCurrentLevel },
		{ .NameUTF8 = UTF8TEXT("CreatePackageArchive"), .Pointer = &UVW_Creators_PluginBPLibrary::execCreatePackageArchive },
		{ .NameUTF8 = UTF8TEXT("LoadConfig"), .Pointer = &UVW_Creators_PluginBPLibrary::execLoadConfig },
		{ .NameUTF8 = UTF8TEXT("SaveConfig"), .Pointer = &UVW_Creators_PluginBPLibrary::execSaveConfig },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CancelPackageBuild, "CancelPackageBuild" }, // 3374072000
		{ &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CookCurrentLevel, "CookCurrentLevel" }, // 3068588465
		{ &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_CreatePackageArchive, "CreatePackageArchive" }, // 2049277551
		{ &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_LoadConfig, "LoadConfig" }, // 3062040195
		{ &Z_Construct_UFunction_UVW_Creators_PluginBPLibrary_SaveConfig, "SaveConfig" }, // 1392812824
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVW_Creators_PluginBPLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics
UObject* (*const Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::ClassParams = {
	&UVW_Creators_PluginBPLibrary::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::Class_MetaDataParams)
};
void UVW_Creators_PluginBPLibrary::StaticRegisterNativesUVW_Creators_PluginBPLibrary()
{
	UClass* Class = UVW_Creators_PluginBPLibrary::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::Funcs));
}
UClass* Z_Construct_UClass_UVW_Creators_PluginBPLibrary()
{
	if (!Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary.OuterSingleton, Z_Construct_UClass_UVW_Creators_PluginBPLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVW_Creators_PluginBPLibrary);
UVW_Creators_PluginBPLibrary::~UVW_Creators_PluginBPLibrary() {}
// ********** End Class UVW_Creators_PluginBPLibrary ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVW_Creators_PluginBPLibrary, UVW_Creators_PluginBPLibrary::StaticClass, TEXT("UVW_Creators_PluginBPLibrary"), &Z_Registration_Info_UClass_UVW_Creators_PluginBPLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVW_Creators_PluginBPLibrary), 4111557048U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h__Script_VW_Creators_Plugin_2840529273{
	TEXT("/Script/VW_Creators_Plugin"),
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h__Script_VW_Creators_Plugin_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_VW_Creators_PluginBPLibrary_h__Script_VW_Creators_Plugin_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
