// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SampleEditor : ModuleRules
{
    public SampleEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
                "Core",
            }
        );


        PrivateDependencyModuleNames.AddRange(new string[] {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "Blutility",
                "UMG",
                "OculusXRScene",
                "OculusXRAnchors",
                "MRUtilityKit"
            }
        );
    }
}
