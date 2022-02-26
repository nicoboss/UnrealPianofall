// MIT Licensed, Nico Bosshard 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealPianofallTarget : TargetRules
{
	public UnrealPianofallTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "UnrealPianofall" } );
	}
}
