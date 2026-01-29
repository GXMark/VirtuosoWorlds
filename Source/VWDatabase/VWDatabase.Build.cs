using System.IO;
using UnrealBuildTool;

public class VWDatabase : ModuleRules
{
    public VWDatabase(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Json", 
                "JsonUtilities",
                "VWShared"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );
        
      // Enable exceptions
      bEnableExceptions = true;

      // Add this if ObjectBox has its own macro to disable exceptions (optional)
      PublicDefinitions.Add("OBX_DISABLE_EXCEPTIONS=0");

      string ThirdPartyPath = Path.Combine(ModuleDirectory, "ThirdParty", "ObjectBox");
      string LibPath = Path.Combine(ThirdPartyPath, "lib");
      string IncludePath = Path.Combine(ThirdPartyPath, "include");

      PublicIncludePaths.Add(IncludePath);
      PublicAdditionalLibraries.Add(Path.Combine(LibPath, "objectbox.lib"));
      PublicDelayLoadDLLs.Add("objectbox.dll");

      // Add runtime dependencies when not in editor
      RuntimeDependencies.Add("$(ProjectDir)/Binaries/Win64/objectbox.dll");
    }
}