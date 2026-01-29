// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPMeshComponent.h"
#include "Model/Package/MPAssetRef.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPMeshComponent() {}

// ********** Begin Cross Module References ********************************************************
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPAssetRef();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPMeshComponent();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPMeshComponent **************************************************
struct Z_Construct_UScriptStruct_FMPMeshComponent_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPMeshComponent); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPMeshComponent); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPMeshComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_mesh_ref_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPMeshComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_material_refs_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPMeshComponent.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPMeshComponent constinit property declarations ******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_mesh_ref;
	static const UECodeGen_Private::FStructPropertyParams NewProp_material_refs_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_material_refs;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPMeshComponent constinit property declarations ********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPMeshComponent>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPMeshComponent_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPMeshComponent;
class UScriptStruct* FMPMeshComponent::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPMeshComponent.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPMeshComponent.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPMeshComponent, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPMeshComponent"));
	}
	return Z_Registration_Info_UScriptStruct_FMPMeshComponent.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPMeshComponent Property Definitions *****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewProp_mesh_ref = { "mesh_ref", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMeshComponent, mesh_ref), Z_Construct_UScriptStruct_FMPAssetRef, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_mesh_ref_MetaData), NewProp_mesh_ref_MetaData) }; // 2111211357
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewProp_material_refs_Inner = { "material_refs", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPAssetRef, METADATA_PARAMS(0, nullptr) }; // 2111211357
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewProp_material_refs = { "material_refs", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMeshComponent, material_refs), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_material_refs_MetaData), NewProp_material_refs_MetaData) }; // 2111211357
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPMeshComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewProp_mesh_ref,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewProp_material_refs_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewProp_material_refs,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPMeshComponent_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPMeshComponent Property Definitions *******************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPMeshComponent_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPMeshComponent",
	Z_Construct_UScriptStruct_FMPMeshComponent_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPMeshComponent_Statics::PropPointers),
	sizeof(FMPMeshComponent),
	alignof(FMPMeshComponent),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPMeshComponent_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPMeshComponent_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPMeshComponent()
{
	if (!Z_Registration_Info_UScriptStruct_FMPMeshComponent.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPMeshComponent.InnerSingleton, Z_Construct_UScriptStruct_FMPMeshComponent_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPMeshComponent.InnerSingleton);
}
// ********** End ScriptStruct FMPMeshComponent ****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMeshComponent_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPMeshComponent::StaticStruct, Z_Construct_UScriptStruct_FMPMeshComponent_Statics::NewStructOps, TEXT("MPMeshComponent"),&Z_Registration_Info_UScriptStruct_FMPMeshComponent, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPMeshComponent), 1865356719U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMeshComponent_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMeshComponent_h__Script_VW_Creators_Plugin_895277485{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMeshComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMeshComponent_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
