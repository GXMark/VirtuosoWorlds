// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPSpotLightComponent.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPSpotLightComponent() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FColor();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPSpotLightComponent();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPSpotLightComponent *********************************************
struct Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPSpotLightComponent); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPSpotLightComponent); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_intensity_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_units_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_color_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_attenuation_radius_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_inner_cone_angle_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_outer_cone_angle_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_source_radius_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_soft_source_radius_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_source_length_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_use_temperature_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_temperature_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_affects_world_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_cast_shadows_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_indirect_light_intensity_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_volumetric_scatter_intensity_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSpotLightComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPSpotLightComponent constinit property declarations *************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_intensity;
	static const UECodeGen_Private::FNamePropertyParams NewProp_units;
	static const UECodeGen_Private::FStructPropertyParams NewProp_color;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_attenuation_radius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_inner_cone_angle;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_outer_cone_angle;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_source_radius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_soft_source_radius;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_source_length;
	static void NewProp_use_temperature_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_use_temperature;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_temperature;
	static void NewProp_affects_world_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_affects_world;
	static void NewProp_cast_shadows_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_cast_shadows;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_indirect_light_intensity;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_volumetric_scatter_intensity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPSpotLightComponent constinit property declarations ***************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPSpotLightComponent>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPSpotLightComponent;
class UScriptStruct* FMPSpotLightComponent::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPSpotLightComponent.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPSpotLightComponent.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPSpotLightComponent, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPSpotLightComponent"));
	}
	return Z_Registration_Info_UScriptStruct_FMPSpotLightComponent.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPSpotLightComponent Property Definitions ************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_intensity = { "intensity", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, intensity), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_intensity_MetaData), NewProp_intensity_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_units = { "units", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, units), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_units_MetaData), NewProp_units_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_color = { "color", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, color), Z_Construct_UScriptStruct_FColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_color_MetaData), NewProp_color_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_attenuation_radius = { "attenuation_radius", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, attenuation_radius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_attenuation_radius_MetaData), NewProp_attenuation_radius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_inner_cone_angle = { "inner_cone_angle", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, inner_cone_angle), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_inner_cone_angle_MetaData), NewProp_inner_cone_angle_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_outer_cone_angle = { "outer_cone_angle", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, outer_cone_angle), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_outer_cone_angle_MetaData), NewProp_outer_cone_angle_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_source_radius = { "source_radius", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, source_radius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_source_radius_MetaData), NewProp_source_radius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_soft_source_radius = { "soft_source_radius", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, soft_source_radius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_soft_source_radius_MetaData), NewProp_soft_source_radius_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_source_length = { "source_length", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, source_length), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_source_length_MetaData), NewProp_source_length_MetaData) };
void Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_use_temperature_SetBit(void* Obj)
{
	((FMPSpotLightComponent*)Obj)->use_temperature = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_use_temperature = { "use_temperature", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FMPSpotLightComponent), &Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_use_temperature_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_use_temperature_MetaData), NewProp_use_temperature_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_temperature = { "temperature", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, temperature), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_temperature_MetaData), NewProp_temperature_MetaData) };
void Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_affects_world_SetBit(void* Obj)
{
	((FMPSpotLightComponent*)Obj)->affects_world = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_affects_world = { "affects_world", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FMPSpotLightComponent), &Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_affects_world_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_affects_world_MetaData), NewProp_affects_world_MetaData) };
void Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_cast_shadows_SetBit(void* Obj)
{
	((FMPSpotLightComponent*)Obj)->cast_shadows = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_cast_shadows = { "cast_shadows", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FMPSpotLightComponent), &Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_cast_shadows_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_cast_shadows_MetaData), NewProp_cast_shadows_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_indirect_light_intensity = { "indirect_light_intensity", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, indirect_light_intensity), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_indirect_light_intensity_MetaData), NewProp_indirect_light_intensity_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_volumetric_scatter_intensity = { "volumetric_scatter_intensity", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSpotLightComponent, volumetric_scatter_intensity), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_volumetric_scatter_intensity_MetaData), NewProp_volumetric_scatter_intensity_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_intensity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_units,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_color,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_attenuation_radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_inner_cone_angle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_outer_cone_angle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_source_radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_soft_source_radius,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_source_length,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_use_temperature,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_temperature,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_affects_world,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_cast_shadows,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_indirect_light_intensity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewProp_volumetric_scatter_intensity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPSpotLightComponent Property Definitions **************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPSpotLightComponent",
	Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::PropPointers),
	sizeof(FMPSpotLightComponent),
	alignof(FMPSpotLightComponent),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPSpotLightComponent()
{
	if (!Z_Registration_Info_UScriptStruct_FMPSpotLightComponent.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPSpotLightComponent.InnerSingleton, Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPSpotLightComponent.InnerSingleton);
}
// ********** End ScriptStruct FMPSpotLightComponent ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSpotLightComponent_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPSpotLightComponent::StaticStruct, Z_Construct_UScriptStruct_FMPSpotLightComponent_Statics::NewStructOps, TEXT("MPSpotLightComponent"),&Z_Registration_Info_UScriptStruct_FMPSpotLightComponent, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPSpotLightComponent), 710856810U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSpotLightComponent_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSpotLightComponent_h__Script_VW_Creators_Plugin_1973930122{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSpotLightComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSpotLightComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
