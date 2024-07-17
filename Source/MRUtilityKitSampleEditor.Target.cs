using UnrealBuildTool;
using System.Collections.Generic;

public class MRUtilityKitSampleEditorTarget : TargetRules
{
    public MRUtilityKitSampleEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "MRUtilityKitSample" });
    }
}
