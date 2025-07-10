using UnrealBuildTool;

public class RigelEditor: ModuleRules
{
    public RigelEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
        new string[]
           {
                "Core",
                "CoreUObject",
                "Engine",
                "ApplicationCore",
                "Slate",
                "SlateCore",
                "Rigel",
                "UnrealEd",
                "SceneOutliner",
                "PropertyEditor"
               
               // ... add private dependencies that you statically link with here ...
           });
    }
}
