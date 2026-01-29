// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VirtuosoWorldsEditorTarget : TargetRules
{
	public VirtuosoWorldsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.AddRange(new string[] { "VirtuosoWorlds" });
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		// This includes all modules such that we can play as listener server from editor.
		ExtraModuleNames.AddRange(new string[]
		{
			"VWShared",
			"VWDatabase",
			"VWServer", "VWClient"
		});
	}
}
