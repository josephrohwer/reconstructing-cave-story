// Copyright Joseph Rohwer 2017

using UnrealBuildTool;
using System.Collections.Generic;

public class EscapeTarget : TargetRules
{
	public EscapeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Escape" } );
	}
}
