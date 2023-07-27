// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeadParadiseEditor : ModuleRules
{
	public DeadParadiseEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UnrealEd", "DeadParadise" });
    }
}
