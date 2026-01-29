// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VirtuosoWorldsServerTarget : TargetRules
{
	public VirtuosoWorldsServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.AddRange(new string[] { "VirtuosoWorlds" });
		RegisterModulesCreatedByRider();
		
		// Explicitly disable I/O Store and pakfile support.
		// Additionally, set bUseIoStore=False in the DefaultGame.ini.
  	 	GlobalDefinitions.Add("USE_IOSTORE=0");
		GlobalDefinitions.Add("WITH_PAKFILE=1");
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[]
		{
			"VWShared",
			"VWDatabase",
			"VWServer"
		});
	}
}