// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SaguniStudio_AI : ModuleRules
{
	public SaguniStudio_AI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"AIModule", 
            "GameplayTasks", 
            "NavigationSystem"
		});
	}
}
