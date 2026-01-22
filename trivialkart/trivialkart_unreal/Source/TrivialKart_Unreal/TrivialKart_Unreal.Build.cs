using UnrealBuildTool;

public class TrivialKart_Unreal : ModuleRules
{
	public TrivialKart_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"Paper2D",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"OnlineSubsystem", 
			"OnlineSubsystemUtils" 
		});
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			DynamicallyLoadedModuleNames.Add("OnlineSubsystemGooglePlay");
		}
	}
}
