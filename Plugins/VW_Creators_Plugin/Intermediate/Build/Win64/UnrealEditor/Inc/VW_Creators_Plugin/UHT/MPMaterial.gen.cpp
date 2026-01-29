// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPMaterial.h"
#include "Model/Package/MPLinearColor.h"
#include "Model/Package/MPScalar.h"
#include "Model/Package/MPTexture.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPMaterial() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPLinearColor();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPMaterial();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPScalar();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPTexture();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPMaterial *******************************************************
struct Z_Construct_UScriptStruct_FMPMaterial_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPMaterial); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPMaterial); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_id_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Material guid identifier\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Material guid identifier" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_name_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Material property name\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Material property name" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_parent_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// (Master) Material parent name\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "(Master) Material parent name" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_scalars_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Material scalar (float) values\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Material scalar (float) values" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_vectors_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Material vectors\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Material vectors" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_textures_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Material texture info\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPMaterial.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Material texture info" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPMaterial constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_id;
	static const UECodeGen_Private::FNamePropertyParams NewProp_name;
	static const UECodeGen_Private::FNamePropertyParams NewProp_parent;
	static const UECodeGen_Private::FStructPropertyParams NewProp_scalars_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_scalars;
	static const UECodeGen_Private::FStructPropertyParams NewProp_vectors_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_vectors;
	static const UECodeGen_Private::FStructPropertyParams NewProp_textures_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_textures;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPMaterial constinit property declarations *************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPMaterial>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPMaterial_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPMaterial;
class UScriptStruct* FMPMaterial::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPMaterial.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPMaterial.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPMaterial, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPMaterial"));
	}
	return Z_Registration_Info_UScriptStruct_FMPMaterial.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPMaterial Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_id = { "id", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMaterial, id), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_id_MetaData), NewProp_id_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMaterial, name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_name_MetaData), NewProp_name_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_parent = { "parent", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMaterial, parent), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_parent_MetaData), NewProp_parent_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_scalars_Inner = { "scalars", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPScalar, METADATA_PARAMS(0, nullptr) }; // 852901822
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_scalars = { "scalars", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMaterial, scalars), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_scalars_MetaData), NewProp_scalars_MetaData) }; // 852901822
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_vectors_Inner = { "vectors", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPLinearColor, METADATA_PARAMS(0, nullptr) }; // 389099508
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_vectors = { "vectors", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMaterial, vectors), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_vectors_MetaData), NewProp_vectors_MetaData) }; // 389099508
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_textures_Inner = { "textures", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPTexture, METADATA_PARAMS(0, nullptr) }; // 4063883230
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_textures = { "textures", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPMaterial, textures), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_textures_MetaData), NewProp_textures_MetaData) }; // 4063883230
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPMaterial_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_id,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_parent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_scalars_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_scalars,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_vectors_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_vectors,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_textures_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPMaterial_Statics::NewProp_textures,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPMaterial_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPMaterial Property Definitions ************************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPMaterial_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPMaterial",
	Z_Construct_UScriptStruct_FMPMaterial_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPMaterial_Statics::PropPointers),
	sizeof(FMPMaterial),
	alignof(FMPMaterial),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPMaterial_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPMaterial_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPMaterial()
{
	if (!Z_Registration_Info_UScriptStruct_FMPMaterial.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPMaterial.InnerSingleton, Z_Construct_UScriptStruct_FMPMaterial_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPMaterial.InnerSingleton);
}
// ********** End ScriptStruct FMPMaterial *********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMaterial_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPMaterial::StaticStruct, Z_Construct_UScriptStruct_FMPMaterial_Statics::NewStructOps, TEXT("MPMaterial"),&Z_Registration_Info_UScriptStruct_FMPMaterial, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPMaterial), 1928443431U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMaterial_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMaterial_h__Script_VW_Creators_Plugin_2386112760{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMaterial_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPMaterial_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
