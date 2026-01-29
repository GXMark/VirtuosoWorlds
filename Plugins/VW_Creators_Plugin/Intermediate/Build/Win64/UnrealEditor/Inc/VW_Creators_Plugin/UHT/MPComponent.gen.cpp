// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPComponent.h"
#include "Model/Package/MPDecalComponent.h"
#include "Model/Package/MPMeshComponent.h"
#include "Model/Package/MPPointLightComponent.h"
#include "Model/Package/MPSpotLightComponent.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPComponent() {}

// ********** Begin Cross Module References ********************************************************
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPComponent();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPDecalComponent();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPMeshComponent();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPPointLightComponent();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPSpotLightComponent();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPComponent ******************************************************
struct Z_Construct_UScriptStruct_FMPComponent_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPComponent); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPComponent); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mesh_comp_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_decal_comp_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// mesh component\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "mesh component" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_point_light_comp_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// decal component\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "decal component" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_spot_light_comp_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// point light component\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "point light component" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPComponent constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_mesh_comp;
	static const UECodeGen_Private::FStructPropertyParams NewProp_decal_comp;
	static const UECodeGen_Private::FStructPropertyParams NewProp_point_light_comp;
	static const UECodeGen_Private::FStructPropertyParams NewProp_spot_light_comp;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPComponent constinit property declarations ************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPComponent>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPComponent_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPComponent;
class UScriptStruct* FMPComponent::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPComponent.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPComponent.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPComponent, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPComponent"));
	}
	return Z_Registration_Info_UScriptStruct_FMPComponent.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPComponent Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_mesh_comp = { "mesh_comp", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPComponent, mesh_comp), Z_Construct_UScriptStruct_FMPMeshComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mesh_comp_MetaData), NewProp_mesh_comp_MetaData) }; // 1865356719
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_decal_comp = { "decal_comp", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPComponent, decal_comp), Z_Construct_UScriptStruct_FMPDecalComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_decal_comp_MetaData), NewProp_decal_comp_MetaData) }; // 707116778
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_point_light_comp = { "point_light_comp", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPComponent, point_light_comp), Z_Construct_UScriptStruct_FMPPointLightComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_point_light_comp_MetaData), NewProp_point_light_comp_MetaData) }; // 3832565322
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_spot_light_comp = { "spot_light_comp", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPComponent, spot_light_comp), Z_Construct_UScriptStruct_FMPSpotLightComponent, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_spot_light_comp_MetaData), NewProp_spot_light_comp_MetaData) }; // 710856810
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_mesh_comp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_decal_comp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_point_light_comp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPComponent_Statics::NewProp_spot_light_comp,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPComponent_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPComponent Property Definitions ***********************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPComponent_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPComponent",
	Z_Construct_UScriptStruct_FMPComponent_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPComponent_Statics::PropPointers),
	sizeof(FMPComponent),
	alignof(FMPComponent),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPComponent_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPComponent_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPComponent()
{
	if (!Z_Registration_Info_UScriptStruct_FMPComponent.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPComponent.InnerSingleton, Z_Construct_UScriptStruct_FMPComponent_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPComponent.InnerSingleton);
}
// ********** End ScriptStruct FMPComponent ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPComponent_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPComponent::StaticStruct, Z_Construct_UScriptStruct_FMPComponent_Statics::NewStructOps, TEXT("MPComponent"),&Z_Registration_Info_UScriptStruct_FMPComponent, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPComponent), 2251359616U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPComponent_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPComponent_h__Script_VW_Creators_Plugin_1525195934{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
