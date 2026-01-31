using UnrealBuildTool;

public class VWClient : ModuleRules
{
    public VWClient(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "RenderCore",
                "RHI",
                "PakFile",
                "VWShared"
            }
        );
                
        if (Target.Type != TargetType.Server)
        {
            // Add runtime dependencies when not in editor
            RuntimeDependencies.Add("$(ProjectDir)/Binaries/Win64/crnlib.dll");
        }
    }
}
