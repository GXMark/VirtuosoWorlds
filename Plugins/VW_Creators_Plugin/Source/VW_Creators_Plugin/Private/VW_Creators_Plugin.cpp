// Copyright Epic Games, Inc. All Rights Reserved.

#include "VW_Creators_Plugin.h"

#define LOCTEXT_NAMESPACE "FVW_Creators_PluginModule"

void FVW_Creators_PluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FVW_Creators_PluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVW_Creators_PluginModule, VW_Creators_Plugin)