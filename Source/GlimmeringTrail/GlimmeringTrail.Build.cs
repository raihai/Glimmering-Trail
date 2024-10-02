// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GlimmeringTrail : ModuleRules
{
	public GlimmeringTrail(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "CustomStateMachine", "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
