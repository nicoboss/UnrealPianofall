// MIT Licensed, Nico Bosshard 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealPianofallEditorTarget : TargetRules
{
	public UnrealPianofallEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "UnrealPianofall" } );
	}
}
