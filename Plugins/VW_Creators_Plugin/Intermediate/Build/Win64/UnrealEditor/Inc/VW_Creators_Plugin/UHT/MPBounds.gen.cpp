// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Model/Package/MPBounds.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeMPBounds() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector3f();
UPackage* Z_Construct_UPackage__Script_VW_Creators_Plugin();
VW_CREATORS_PLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMPBounds();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FMPBounds *********************************************************
struct Z_Construct_UScriptStruct_FMPBounds_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FMPBounds); }
	static inline consteval int16 GetStructAlignment() { return alignof(FMPBounds); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPBounds.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_min_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPBounds.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_max_MetaData[] = {
		{ "ModuleRelativePath", "Public/Model/Package/MPBounds.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FMPBounds constinit property declarations *************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_min;
	static const UECodeGen_Private::FStructPropertyParams NewProp_max;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FMPBounds constinit property declarations ***************************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMPBounds>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FMPBounds_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FMPBounds;
class UScriptStruct* FMPBounds::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FMPBounds.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FMPBounds.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMPBounds, (UObject*)Z_Construct_UPackage__Script_VW_Creators_Plugin(), TEXT("MPBounds"));
	}
	return Z_Registration_Info_UScriptStruct_FMPBounds.OuterSingleton;
	}

// ********** Begin ScriptStruct FMPBounds Property Definitions ************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPBounds_Statics::NewProp_min = { "min", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPBounds, min), Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_min_MetaData), NewProp_min_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMPBounds_Statics::NewProp_max = { "max", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMPBounds, max), Z_Construct_UScriptStruct_FVector3f, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_max_MetaData), NewProp_max_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMPBounds_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPBounds_Statics::NewProp_min,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMPBounds_Statics::NewProp_max,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPBounds_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FMPBounds Property Definitions **************************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMPBounds_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VW_Creators_Plugin,
	nullptr,
	&NewStructOps,
	"MPBounds",
	Z_Construct_UScriptStruct_FMPBounds_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPBounds_Statics::PropPointers),
	sizeof(FMPBounds),
	alignof(FMPBounds),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMPBounds_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMPBounds_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMPBounds()
{
	if (!Z_Registration_Info_UScriptStruct_FMPBounds.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FMPBounds.InnerSingleton, Z_Construct_UScriptStruct_FMPBounds_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FMPBounds.InnerSingleton);
}
// ********** End ScriptStruct FMPBounds ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPBounds_h__Script_VW_Creators_Plugin_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMPBounds::StaticStruct, Z_Construct_UScriptStruct_FMPBounds_Statics::NewStructOps, TEXT("MPBounds"),&Z_Registration_Info_UScriptStruct_FMPBounds, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMPBounds), 201115156U) },
	};
}; // Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPBounds_h__Script_VW_Creators_Plugin_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPBounds_h__Script_VW_Creators_Plugin_436500342{
	TEXT("/Script/VW_Creators_Plugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPBounds_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_VGS_Development_Unreal_Projects_VirtuosoWorlds_Plugins_VW_Creators_Plugin_Source_VW_Creators_Plugin_Public_Model_Package_MPBounds_h__Script_VW_Creators_Plugin_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
