// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPConvexCollision.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPConvexCollision() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPConvexCollision();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPConvexCollision ************************************************
struct Z_Construct_UScriptStruct_FMPConvexCollision_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPConvexCollision); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPConvexCollision); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPConvexCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Vertices_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPConvexCollision.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPConvexCollision constinit property declarations ****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Vertices_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Vertices;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPConvexCollision constinit property declarations ******************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPConvexCollision>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPConvexCollision_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPConvexCollision;
class UScriptStruct* FMPConvexCollision::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPConvexCollision.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPConvexCollision.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPConvexCollision, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPConvexCollision"));
	}
	return Z_Registration_Info_UScriptStruct_FMPConvexCollision.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPConvexCollision Property Definitions ***************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPConvexCollision_Statics::NewProp_Vertices_Inner = { "Vertices", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMPConvexCollision_Statics::NewProp_Vertices = { "Vertices", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPConvexCollision, Vertices), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Vertices_MetaData), NewProp_Vertices_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPConvexCollision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPConvexCollision_Statics::NewProp_Vertices_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPConvexCollision_Statics::NewProp_Vertices,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPConvexCollision_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPConvexCollision Property Definitions *****************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPConvexCollision_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPConvexCollision",
	Z_Construct_UScriptStruct_FMPConvexCollision_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPConvexCollision_Statics::PropPointers),
	sizeof(FMPConvexCollision),
	alignof(FMPConvexCollision),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPConvexCollision_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPConvexCollision_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPConvexCollision()
{
	if (!Z_Registration_Info_UScriptStruct_FMPConvexCollision.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPConvexCollision.InnerSingleton, Z_Construct_UScriptStruct_FMPConvexCollision_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPConvexCollision.InnerSingleton);
}
// ********** End ScriptStruct FMPConvexCollision **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPConvexCollision_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPConvexCollision::StaticStruct, Z_Construct_UScriptStruct_FMPConvexCollision_Statics::NewStructOps, TEXT("MPConvexCollision"),&Z_Registration_Info_UScriptStruct_FMPConvexCollision, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPConvexCollision), 776825556U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPConvexCollision_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPConvexCollision_h__Script_VW_Creators_Plugin_1624987195{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPConvexCollision_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPConvexCollision_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
