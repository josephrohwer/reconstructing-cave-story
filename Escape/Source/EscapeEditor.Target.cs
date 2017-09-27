// Copyright Joseph Rohwer 2017

using UnrealBuildTool;
using System.Collections.Generic;

public class EscapeEditorTarget : TargetRules
{
	public EscapeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

        bUseUnityBuild = false;
        bUsePCHFiles = false;

		ExtraModuleNames.AddRange( new string[] { "Escape" } );
	}
}
