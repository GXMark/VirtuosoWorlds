using UnrealBuildTool;

public class VWServer : ModuleRules
{
    public VWServer(ReadOnlyTargetRules Target) : base(Target)
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
		        "VWShared",
                "VWDatabase",
                "ProceduralMeshComponent"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Json",
                "JsonUtilities",
            }
        );
    }
}
