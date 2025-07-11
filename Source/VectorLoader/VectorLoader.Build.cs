using System.IO;
using UnrealBuildTool;

public class VectorLoader: ModuleRules
{
    public VectorLoader(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(PluginDirectory, "../ThirdParty/include")
            }
        );

        string libPath = Path.Combine(PluginDirectory, "../ThirdParty/lib/");
        //string debugPostfix = "d";
        //string libPostfix = ".lib";

        PublicAdditionalLibraries.AddRange(
            new string[]{
                   //libPath + "opencv" + debugPostfix + libPostfix
			}
            );

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "Engine",
            "ProceduralMeshComponent",
            "GDALForUnreal",
            "GeoReferencing",
            "Json"
       });
    }
}
