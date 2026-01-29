// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPLinearColor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPLinearColor() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPLinearColor();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPLinearColor ****************************************************
struct Z_Construct_UScriptStruct_FMPLinearColor_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPLinearColor); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPLinearColor); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPLinearColor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_name_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPLinearColor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_color_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPLinearColor.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPLinearColor constinit property declarations ********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_name;
	static const UECodeGen_Private::FStructPropertyParams NewProp_color;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPLinearColor constinit property declarations **********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPLinearColor>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPLinearColor_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPLinearColor;
class UScriptStruct* FMPLinearColor::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPLinearColor.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPLinearColor.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPLinearColor, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPLinearColor"));
	}
	return Z_Registration_Info_UScriptStruct_FMPLinearColor.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPLinearColor Property Definitions *******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPLinearColor_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPLinearColor, name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_name_MetaData), NewProp_name_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPLinearColor_Statics::NewProp_color = { "color", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPLinearColor, color), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_color_MetaData), NewProp_color_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPLinearColor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPLinearColor_Statics::NewProp_name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPLinearColor_Statics::NewProp_color,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPLinearColor_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPLinearColor Property Definitions *********************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPLinearColor_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPLinearColor",
	Z_Construct_UScriptStruct_FMPLinearColor_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPLinearColor_Statics::PropPointers),
	sizeof(FMPLinearColor),
	alignof(FMPLinearColor),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPLinearColor_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPLinearColor_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPLinearColor()
{
	if (!Z_Registration_Info_UScriptStruct_FMPLinearColor.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPLinearColor.InnerSingleton, Z_Construct_UScriptStruct_FMPLinearColor_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPLinearColor.InnerSingleton);
}
// ********** End ScriptStruct FMPLinearColor ******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPLinearColor_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPLinearColor::StaticStruct, Z_Construct_UScriptStruct_FMPLinearColor_Statics::NewStructOps, TEXT("MPLinearColor"),&Z_Registration_Info_UScriptStruct_FMPLinearColor, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPLinearColor), 389099508U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPLinearColor_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPLinearColor_h__Script_VW_Creators_Plugin_3303674328{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPLinearColor_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPLinearColor_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
