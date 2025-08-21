// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Practica : ModuleRules
{
	public Practica(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Practica",
			"Practica/Variant_Platforming",
			"Practica/Variant_Platforming/Animation",
			"Practica/Variant_Combat",
			"Practica/Variant_Combat/AI",
			"Practica/Variant_Combat/Animation",
			"Practica/Variant_Combat/Gameplay",
			"Practica/Variant_Combat/Interfaces",
			"Practica/Variant_Combat/UI",
			"Practica/Variant_SideScrolling",
			"Practica/Variant_SideScrolling/AI",
			"Practica/Variant_SideScrolling/Gameplay",
			"Practica/Variant_SideScrolling/Interfaces",
			"Practica/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
