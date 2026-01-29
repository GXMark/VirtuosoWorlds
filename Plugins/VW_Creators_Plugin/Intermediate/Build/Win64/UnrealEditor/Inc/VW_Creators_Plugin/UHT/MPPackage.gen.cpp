// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPPackage.h"
#include "Model/Package/MPActor.h"
#include "Model/Package/MPCollision.h"
#include "Model/Package/MPMaterial.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPPackage() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPActor();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPCollision();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPMaterial();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPPackage();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPPackage ********************************************************
struct Z_Construct_UScriptStruct_FMPPackage_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPPackage); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPPackage); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_id_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_name_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_description_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_author_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_engine_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_created_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_actors_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_materials_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// root actors (static mesh, hism, decal, light, etc.)\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "root actors (static mesh, hism, decal, light, etc.)" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_collisions_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// shared materials supporting root, hism and group actors\n" },
#endif
		{ "ModuleRelativePath", "Public/Model/Package/MPPackage.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "shared materials supporting root, hism and group actors" },
#endif
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPPackage constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_id;
	static const UECodeGen_Private::FNamePropertyParams NewProp_name;
	static const UECodeGen_Private::FNamePropertyParams NewProp_description;
	static const UECodeGen_Private::FNamePropertyParams NewProp_author;
	static const UECodeGen_Private::FNamePropertyParams NewProp_engine;
	static const UECodeGen_Private::FNamePropertyParams NewProp_created;
	static const UECodeGen_Private::FStructPropertyParams NewProp_actors_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_actors;
	static const UECodeGen_Private::FStructPropertyParams NewProp_materials_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_materials;
	static const UECodeGen_Private::FStructPropertyParams NewProp_collisions_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_collisions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPPackage constinit property declarations **************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPPackage>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPPackage_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPPackage;
class UScriptStruct* FMPPackage::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPPackage.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPPackage.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPPackage, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPPackage"));
	}
	return Z_Registration_Info_UScriptStruct_FMPPackage.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPPackage Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_id = { "id", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, id), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_id_MetaData), NewProp_id_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_name = { "name", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_name_MetaData), NewProp_name_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_description = { "description", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, description), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_description_MetaData), NewProp_description_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_author = { "author", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, author), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_author_MetaData), NewProp_author_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_engine = { "engine", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, engine), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_engine_MetaData), NewProp_engine_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_created = { "created", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, created), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_created_MetaData), NewProp_created_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_actors_Inner = { "actors", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPActor, METADATA_PARAMS(0, nullptr) }; // 2052362053
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_actors = { "actors", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, actors), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_actors_MetaData), NewProp_actors_MetaData) }; // 2052362053
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_materials_Inner = { "materials", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPMaterial, METADATA_PARAMS(0, nullptr) }; // 1928443431
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_materials = { "materials", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, materials), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_materials_MetaData), NewProp_materials_MetaData) }; // 1928443431
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_collisions_Inner = { "collisions", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPCollision, METADATA_PARAMS(0, nullptr) }; // 120468690
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_collisions = { "collisions", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPPackage, collisions), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_collisions_MetaData), NewProp_collisions_MetaData) }; // 120468690
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPPackage_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_id,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_description,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_author,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_engine,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_created,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_actors_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_actors,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_materials_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_materials,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_collisions_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPPackage_Statics::NewProp_collisions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPPackage_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPPackage Property Definitions *************************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPPackage_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPPackage",
	Z_Construct_UScriptStruct_FMPPackage_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPPackage_Statics::PropPointers),
	sizeof(FMPPackage),
	alignof(FMPPackage),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPPackage_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPPackage_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPPackage()
{
	if (!Z_Registration_Info_UScriptStruct_FMPPackage.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPPackage.InnerSingleton, Z_Construct_UScriptStruct_FMPPackage_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPPackage.InnerSingleton);
}
// ********** End ScriptStruct FMPPackage **********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPPackage_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPPackage::StaticStruct, Z_Construct_UScriptStruct_FMPPackage_Statics::NewStructOps, TEXT("MPPackage"),&Z_Registration_Info_UScriptStruct_FMPPackage, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPPackage), 4227072870U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPPackage_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPPackage_h__Script_VW_Creators_Plugin_2099910173{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPPackage_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPPackage_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
