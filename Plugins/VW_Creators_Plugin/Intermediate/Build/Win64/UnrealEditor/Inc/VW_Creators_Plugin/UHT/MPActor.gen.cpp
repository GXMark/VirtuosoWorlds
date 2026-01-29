// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPActor.h"
#include "Model/Package/MPComponent.h"
#include "Model/Package/MPTransform.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPActor() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPActor();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPComponent();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPTransform();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPActor **********************************************************
struct Z_Construct_UScriptStruct_FMPActor_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPActor); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPActor); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_id_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_pid_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_name_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_type_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_transform_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_component_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPActor constinit property declarations **************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_id;
	static const UECodeGen_Private::FStructPropertyParams NewProp_pid;
	static const UECodeGen_Private::FNamePropertyParams NewProp_name;
	static const UECodeGen_Private::FNamePropertyParams NewProp_type;
	static const UECodeGen_Private::FStructPropertyParams NewProp_transform;
	static const UECodeGen_Private::FStructPropertyParams NewProp_component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPActor constinit property declarations ****************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPActor>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPActor_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPActor;
class UScriptStruct* FMPActor::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPActor.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPActor.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPActor, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPActor"));
	}
	return Z_Registration_Info_UScriptStruct_FMPActor.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPActor Property Definitions *************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_id = { "id", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPActor, id), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_id_MetaData), NewProp_id_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_pid = { "pid", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPActor, pid), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_pid_MetaData), NewProp_pid_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPActor, name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_name_MetaData), NewProp_name_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_type = { "type", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPActor, type), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_type_MetaData), NewProp_type_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_transform = { "transform", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPActor, transform), Z_Construct_UScriptStruct_FMPTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_transform_MetaData), NewProp_transform_MetaData) }; // 240261705
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_component = { "component", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPActor, component), Z_Construct_UScriptStruct_FMPComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_component_MetaData), NewProp_component_MetaData) }; // 2251359616
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_id,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_pid,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_type,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_transform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPActor_Statics::NewProp_component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPActor_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPActor Property Definitions ***************************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPActor_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPActor",
	Z_Construct_UScriptStruct_FMPActor_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPActor_Statics::PropPointers),
	sizeof(FMPActor),
	alignof(FMPActor),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPActor_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPActor_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPActor()
{
	if (!Z_Registration_Info_UScriptStruct_FMPActor.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPActor.InnerSingleton, Z_Construct_UScriptStruct_FMPActor_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPActor.InnerSingleton);
}
// ********** End ScriptStruct FMPActor ************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPActor_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPActor::StaticStruct, Z_Construct_UScriptStruct_FMPActor_Statics::NewStructOps, TEXT("MPActor"),&Z_Registration_Info_UScriptStruct_FMPActor, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPActor), 1290168997U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPActor_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPActor_h__Script_VW_Creators_Plugin_690639581{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPActor_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPActor_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
