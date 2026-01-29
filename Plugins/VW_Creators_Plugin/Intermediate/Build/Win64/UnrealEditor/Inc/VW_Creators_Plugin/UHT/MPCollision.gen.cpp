// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPCollision.h"
#include "Model/Package/MPBoxCollision.h"
#include "Model/Package/MPCapsuleCollision.h"
#include "Model/Package/MPConvexCollision.h"
#include "Model/Package/MPSphereCollision.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPCollision() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPBoxCollision();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPCapsuleCollision();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPCollision();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPConvexCollision();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPSphereCollision();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPCollision ******************************************************
struct Z_Construct_UScriptStruct_FMPCollision_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPCollision); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPCollision); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MeshID_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Spheres_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Boxes_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Capsules_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Convexes_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPCollision.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPCollision constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_MeshID;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Spheres_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Spheres;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Boxes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Boxes;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Capsules_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Capsules;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Convexes_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Convexes;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPCollision constinit property declarations ************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPCollision>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPCollision_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPCollision;
class UScriptStruct* FMPCollision::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPCollision.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPCollision.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPCollision, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPCollision"));
	}
	return Z_Registration_Info_UScriptStruct_FMPCollision.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPCollision Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_MeshID = { "MeshID", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPCollision, MeshID), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MeshID_MetaData), NewProp_MeshID_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Spheres_Inner = { "Spheres", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPSphereCollision, METADATA_PARAMS(0, nullptr) }; // 336691558
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Spheres = { "Spheres", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPCollision, Spheres), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Spheres_MetaData), NewProp_Spheres_MetaData) }; // 336691558
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Boxes_Inner = { "Boxes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPBoxCollision, METADATA_PARAMS(0, nullptr) }; // 3097901127
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Boxes = { "Boxes", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPCollision, Boxes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Boxes_MetaData), NewProp_Boxes_MetaData) }; // 3097901127
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Capsules_Inner = { "Capsules", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPCapsuleCollision, METADATA_PARAMS(0, nullptr) }; // 3246374723
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Capsules = { "Capsules", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPCollision, Capsules), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Capsules_MetaData), NewProp_Capsules_MetaData) }; // 3246374723
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Convexes_Inner = { "Convexes", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FMPConvexCollision, METADATA_PARAMS(0, nullptr) }; // 776825556
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Convexes = { "Convexes", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPCollision, Convexes), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Convexes_MetaData), NewProp_Convexes_MetaData) }; // 776825556
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPCollision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_MeshID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Spheres_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Spheres,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Boxes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Boxes,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Capsules_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Capsules,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Convexes_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPCollision_Statics::NewProp_Convexes,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPCollision_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPCollision Property Definitions ***********************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPCollision_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPCollision",
	Z_Construct_UScriptStruct_FMPCollision_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPCollision_Statics::PropPointers),
	sizeof(FMPCollision),
	alignof(FMPCollision),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPCollision_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPCollision_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPCollision()
{
	if (!Z_Registration_Info_UScriptStruct_FMPCollision.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPCollision.InnerSingleton, Z_Construct_UScriptStruct_FMPCollision_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPCollision.InnerSingleton);
}
// ********** End ScriptStruct FMPCollision ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPCollision_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPCollision::StaticStruct, Z_Construct_UScriptStruct_FMPCollision_Statics::NewStructOps, TEXT("MPCollision"),&Z_Registration_Info_UScriptStruct_FMPCollision, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPCollision), 120468690U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPCollision_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPCollision_h__Script_VW_Creators_Plugin_2270103223{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPCollision_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPCollision_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
