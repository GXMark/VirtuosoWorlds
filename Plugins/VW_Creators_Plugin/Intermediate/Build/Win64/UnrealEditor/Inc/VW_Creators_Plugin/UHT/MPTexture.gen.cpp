// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPTexture.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPTexture() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPTexture();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPTexture ********************************************************
struct Z_Construct_UScriptStruct_FMPTexture_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPTexture); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPTexture); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_id_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_name_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_property_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_width_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_height_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_linear_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_format_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPTexture.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPTexture constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_id;
	static const UECodeGen_Private::FNamePropertyParams NewProp_name;
	static const UECodeGen_Private::FNamePropertyParams NewProp_property;
	static const UECodeGen_Private::FIntPropertyParams NewProp_width;
	static const UECodeGen_Private::FIntPropertyParams NewProp_height;
	static void NewProp_linear_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_linear;
	static const UECodeGen_Private::FNamePropertyParams NewProp_format;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPTexture constinit property declarations **************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPTexture>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPTexture_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPTexture;
class UScriptStruct* FMPTexture::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPTexture.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPTexture.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPTexture, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPTexture"));
	}
	return Z_Registration_Info_UScriptStruct_FMPTexture.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPTexture Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_id = { "id", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPTexture, id), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_id_MetaData), NewProp_id_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPTexture, name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_name_MetaData), NewProp_name_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_property = { "property", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPTexture, property), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_property_MetaData), NewProp_property_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_width = { "width", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPTexture, width), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_width_MetaData), NewProp_width_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_height = { "height", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPTexture, height), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_height_MetaData), NewProp_height_MetaData) };
void Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_linear_SetBit(void* Obj)
{
	((FMPTexture*)Obj)->linear = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_linear = { "linear", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FMPTexture), &Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_linear_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_linear_MetaData), NewProp_linear_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_format = { "format", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPTexture, format), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_format_MetaData), NewProp_format_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPTexture_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_id,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_property,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_width,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_height,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_linear,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPTexture_Statics::NewProp_format,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPTexture_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPTexture Property Definitions *************************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPTexture_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPTexture",
	Z_Construct_UScriptStruct_FMPTexture_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPTexture_Statics::PropPointers),
	sizeof(FMPTexture),
	alignof(FMPTexture),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPTexture_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPTexture_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPTexture()
{
	if (!Z_Registration_Info_UScriptStruct_FMPTexture.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPTexture.InnerSingleton, Z_Construct_UScriptStruct_FMPTexture_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPTexture.InnerSingleton);
}
// ********** End ScriptStruct FMPTexture **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPTexture_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPTexture::StaticStruct, Z_Construct_UScriptStruct_FMPTexture_Statics::NewStructOps, TEXT("MPTexture"),&Z_Registration_Info_UScriptStruct_FMPTexture, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPTexture), 4063883230U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPTexture_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPTexture_h__Script_VW_Creators_Plugin_1623031997{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPTexture_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPTexture_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
