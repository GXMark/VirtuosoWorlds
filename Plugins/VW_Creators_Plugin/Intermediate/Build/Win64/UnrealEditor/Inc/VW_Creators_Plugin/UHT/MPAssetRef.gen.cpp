// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPAssetRef.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPAssetRef() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPAssetRef();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPAssetRef *******************************************************
struct Z_Construct_UScriptStruct_FMPAssetRef_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPAssetRef); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPAssetRef); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPAssetRef.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_id_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPAssetRef.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_name_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPAssetRef.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_type_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPAssetRef.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_path_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPAssetRef.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_meta_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPAssetRef.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPAssetRef constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_id;
	static const UECodeGen_Private::FNamePropertyParams NewProp_name;
	static const UECodeGen_Private::FNamePropertyParams NewProp_type;
	static const UECodeGen_Private::FNamePropertyParams NewProp_path;
	static const UECodeGen_Private::FNamePropertyParams NewProp_meta;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPAssetRef constinit property declarations *************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPAssetRef>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPAssetRef_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPAssetRef;
class UScriptStruct* FMPAssetRef::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPAssetRef.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPAssetRef.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPAssetRef, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPAssetRef"));
	}
	return Z_Registration_Info_UScriptStruct_FMPAssetRef.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPAssetRef Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_id = { "id", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPAssetRef, id), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_id_MetaData), NewProp_id_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPAssetRef, name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_name_MetaData), NewProp_name_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_type = { "type", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPAssetRef, type), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_type_MetaData), NewProp_type_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_path = { "path", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPAssetRef, path), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_path_MetaData), NewProp_path_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_meta = { "meta", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPAssetRef, meta), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_meta_MetaData), NewProp_meta_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPAssetRef_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_id,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_type,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_path,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewProp_meta,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPAssetRef_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPAssetRef Property Definitions ************************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPAssetRef_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPAssetRef",
	Z_Construct_UScriptStruct_FMPAssetRef_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPAssetRef_Statics::PropPointers),
	sizeof(FMPAssetRef),
	alignof(FMPAssetRef),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPAssetRef_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPAssetRef_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPAssetRef()
{
	if (!Z_Registration_Info_UScriptStruct_FMPAssetRef.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPAssetRef.InnerSingleton, Z_Construct_UScriptStruct_FMPAssetRef_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPAssetRef.InnerSingleton);
}
// ********** End ScriptStruct FMPAssetRef *********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPAssetRef_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPAssetRef::StaticStruct, Z_Construct_UScriptStruct_FMPAssetRef_Statics::NewStructOps, TEXT("MPAssetRef"),&Z_Registration_Info_UScriptStruct_FMPAssetRef, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPAssetRef), 2111211357U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPAssetRef_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPAssetRef_h__Script_VW_Creators_Plugin_352025{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPAssetRef_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPAssetRef_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
