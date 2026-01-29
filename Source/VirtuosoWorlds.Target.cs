// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VirtuosoWorldsTarget : TargetRules
{
	public VirtuosoWorldsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

		ExtraModuleNames.AddRange(new string[] { "VirtuosoWorlds" });
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[]
		{
			"VWShared", "VWClient"
		});
	}
}
