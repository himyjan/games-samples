using UnrealBuildTool;
using System.Collections.Generic;

public class TrivialKart_UnrealTarget : TargetRules
{
	public TrivialKart_UnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("TrivialKart_Unreal");
	}
}
