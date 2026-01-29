using UnrealBuildTool;

public class VWShared : ModuleRules
{
    public VWShared(ReadOnlyTargetRules Target) : base(Target)
    {
        //SetupIrisSupport(Target);
        
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "NetCore",
                "Json",
                "JsonUtilities",
                "HTTP"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );

    }
}