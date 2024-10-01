// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomStateMachine : ModuleRules
{
	public CustomStateMachine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
               //  "CustomStateMachine/Public"
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
             //   "CustomStateMachine/Private"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core", "CoreUObject", "Engine", "InputCore",
               
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
               // "CustomStateMachine",

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
