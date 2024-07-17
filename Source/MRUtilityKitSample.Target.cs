using UnrealBuildTool;
using System.Collections.Generic;

public class MRUtilityKitSampleTarget : TargetRules
{
    public MRUtilityKitSampleTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "MRUtilityKitSample" });
    }
}
