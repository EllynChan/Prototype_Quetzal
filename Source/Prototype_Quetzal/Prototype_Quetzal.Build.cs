// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Prototype_Quetzal : ModuleRules
{
	public Prototype_Quetzal(ReadOnlyTargetRules Target) : base(Target)
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
			"Prototype_Quetzal",
			"Prototype_Quetzal/Variant_Platforming",
			"Prototype_Quetzal/Variant_Platforming/Animation",
			"Prototype_Quetzal/Variant_Combat",
			"Prototype_Quetzal/Variant_Combat/AI",
			"Prototype_Quetzal/Variant_Combat/Animation",
			"Prototype_Quetzal/Variant_Combat/Gameplay",
			"Prototype_Quetzal/Variant_Combat/Interfaces",
			"Prototype_Quetzal/Variant_Combat/UI",
			"Prototype_Quetzal/Variant_SideScrolling",
			"Prototype_Quetzal/Variant_SideScrolling/AI",
			"Prototype_Quetzal/Variant_SideScrolling/Gameplay",
			"Prototype_Quetzal/Variant_SideScrolling/Interfaces",
			"Prototype_Quetzal/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
