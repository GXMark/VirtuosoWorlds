// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPDecalComponent.h"
#include "Model/Package/MPAssetRef.h"
#include "Model/Package/MPLinearColor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPDecalComponent() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector3d();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPAssetRef();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPDecalComponent();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPLinearColor();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPDecalComponent *************************************************
struct Z_Construct_UScriptStruct_FMPDecalComponent_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPDecalComponent); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPDecalComponent); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_material_ref_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_size_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_color_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_fade_screen_size_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_fade_in_start_delay_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_fade_in_duration_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_fade_out_start_delay_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_fade_out_duration_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_destroy_after_fade_out_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPDecalComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPDecalComponent constinit property declarations *****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_material_ref;
	static const UECodeGen_Private::FStructPropertyParams NewProp_size;
	static const UECodeGen_Private::FStructPropertyParams NewProp_color;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_fade_screen_size;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_fade_in_start_delay;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_fade_in_duration;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_fade_out_start_delay;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_fade_out_duration;
	static void NewProp_destroy_after_fade_out_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_destroy_after_fade_out;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPDecalComponent constinit property declarations *******************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPDecalComponent>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPDecalComponent_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPDecalComponent;
class UScriptStruct* FMPDecalComponent::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPDecalComponent.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPDecalComponent.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPDecalComponent, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPDecalComponent"));
	}
	return Z_Registration_Info_UScriptStruct_FMPDecalComponent.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPDecalComponent Property Definitions ****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_material_ref = { "material_ref", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, material_ref), Z_Construct_UScriptStruct_FMPAssetRef, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_material_ref_MetaData), NewProp_material_ref_MetaData) }; // 2111211357
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_size = { "size", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, size), Z_Construct_UScriptStruct_FVector3d, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_size_MetaData), NewProp_size_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_color = { "color", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, color), Z_Construct_UScriptStruct_FMPLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_color_MetaData), NewProp_color_MetaData) }; // 389099508
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_screen_size = { "fade_screen_size", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, fade_screen_size), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_fade_screen_size_MetaData), NewProp_fade_screen_size_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_in_start_delay = { "fade_in_start_delay", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, fade_in_start_delay), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_fade_in_start_delay_MetaData), NewProp_fade_in_start_delay_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_in_duration = { "fade_in_duration", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, fade_in_duration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_fade_in_duration_MetaData), NewProp_fade_in_duration_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_out_start_delay = { "fade_out_start_delay", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, fade_out_start_delay), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_fade_out_start_delay_MetaData), NewProp_fade_out_start_delay_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_out_duration = { "fade_out_duration", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPDecalComponent, fade_out_duration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_fade_out_duration_MetaData), NewProp_fade_out_duration_MetaData) };
void Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_destroy_after_fade_out_SetBit(void* Obj)
{
	((FMPDecalComponent*)Obj)->destroy_after_fade_out = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_destroy_after_fade_out = { "destroy_after_fade_out", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FMPDecalComponent), &Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_destroy_after_fade_out_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_destroy_after_fade_out_MetaData), NewProp_destroy_after_fade_out_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPDecalComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_material_ref,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_size,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_color,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_screen_size,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_in_start_delay,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_in_duration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_out_start_delay,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_fade_out_duration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewProp_destroy_after_fade_out,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPDecalComponent_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPDecalComponent Property Definitions ******************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPDecalComponent_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPDecalComponent",
	Z_Construct_UScriptStruct_FMPDecalComponent_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPDecalComponent_Statics::PropPointers),
	sizeof(FMPDecalComponent),
	alignof(FMPDecalComponent),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPDecalComponent_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPDecalComponent_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPDecalComponent()
{
	if (!Z_Registration_Info_UScriptStruct_FMPDecalComponent.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPDecalComponent.InnerSingleton, Z_Construct_UScriptStruct_FMPDecalComponent_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPDecalComponent.InnerSingleton);
}
// ********** End ScriptStruct FMPDecalComponent ***************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPDecalComponent_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPDecalComponent::StaticStruct, Z_Construct_UScriptStruct_FMPDecalComponent_Statics::NewStructOps, TEXT("MPDecalComponent"),&Z_Registration_Info_UScriptStruct_FMPDecalComponent, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPDecalComponent), 707116778U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPDecalComponent_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPDecalComponent_h__Script_VW_Creators_Plugin_4064553884{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPDecalComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPDecalComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
