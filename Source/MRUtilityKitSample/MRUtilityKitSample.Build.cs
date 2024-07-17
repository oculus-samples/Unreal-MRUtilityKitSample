using UnrealBuildTool;

public class MRUtilityKitSample : ModuleRules
{
    public MRUtilityKitSample(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput"
        });

        if (Target.Version.MajorVersion > 5 || (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 3))
        {
            PublicDependencyModuleNames.AddRange(new string[] {
                "XRBase",
            });
        }


	PrivateDependencyModuleNames.AddRange(new string[] {
		"HeadMountedDisplay",
		"UMG",
		"MRUtilityKit",
		"OculusXRHMD",
		"OculusXRAnchors",
		"OculusXRScene",
		"ProceduralMeshComponent",
		"PCG"
	});

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "EditorSubsystem",
                "UMGEditor",
                "Blutility",
            });
        }
    }
}
