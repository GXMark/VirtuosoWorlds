// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VirtuosoWorlds : ModuleRules
{
	public VirtuosoWorlds(ReadOnlyTargetRules Target) : base(Target)
	{
		//SetupIrisSupport(Target);
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"NetCore",
			"InputCore",
			"RenderCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Json",
			"JsonUtilities",
			"HTTP",
			"PakFile",
			"VWShared"
		});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
		);

		PublicIncludePaths.AddRange(new string[]
		{
			"VirtuosoWorlds"
		});

		if (Target.bWithClientCode)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"VWClient"
				});
		}
		
		if (Target.bWithServerCode)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"VWDatabase",
					"VWServer"
				});
		}
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}