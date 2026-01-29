// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPSphereCollision.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPSphereCollision() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPSphereCollision();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPSphereCollision ************************************************
struct Z_Construct_UScriptStruct_FMPSphereCollision_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPSphereCollision); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPSphereCollision); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSphereCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Center_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSphereCollision.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Radius_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPSphereCollision.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPSphereCollision constinit property declarations ****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Center;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Radius;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPSphereCollision constinit property declarations ******************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPSphereCollision>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPSphereCollision_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPSphereCollision;
class UScriptStruct* FMPSphereCollision::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPSphereCollision.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPSphereCollision.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPSphereCollision, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPSphereCollision"));
	}
	return Z_Registration_Info_UScriptStruct_FMPSphereCollision.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPSphereCollision Property Definitions ***************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPSphereCollision_Statics::NewProp_Center = { "Center", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSphereCollision, Center), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Center_MetaData), NewProp_Center_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FMPSphereCollision_Statics::NewProp_Radius = { "Radius", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPSphereCollision, Radius), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Radius_MetaData), NewProp_Radius_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPSphereCollision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSphereCollision_Statics::NewProp_Center,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPSphereCollision_Statics::NewProp_Radius,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPSphereCollision_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPSphereCollision Property Definitions *****************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPSphereCollision_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPSphereCollision",
	Z_Construct_UScriptStruct_FMPSphereCollision_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPSphereCollision_Statics::PropPointers),
	sizeof(FMPSphereCollision),
	alignof(FMPSphereCollision),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPSphereCollision_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPSphereCollision_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPSphereCollision()
{
	if (!Z_Registration_Info_UScriptStruct_FMPSphereCollision.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPSphereCollision.InnerSingleton, Z_Construct_UScriptStruct_FMPSphereCollision_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPSphereCollision.InnerSingleton);
}
// ********** End ScriptStruct FMPSphereCollision **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSphereCollision_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPSphereCollision::StaticStruct, Z_Construct_UScriptStruct_FMPSphereCollision_Statics::NewStructOps, TEXT("MPSphereCollision"),&Z_Registration_Info_UScriptStruct_FMPSphereCollision, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPSphereCollision), 336691558U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSphereCollision_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSphereCollision_h__Script_VW_Creators_Plugin_3707278252{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSphereCollision_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPSphereCollision_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
