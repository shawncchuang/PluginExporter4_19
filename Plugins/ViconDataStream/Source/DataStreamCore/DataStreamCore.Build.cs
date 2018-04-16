using UnrealBuildTool;
using System;
using System.IO;
public class DataStreamCore : ModuleRules
{
  public DataStreamCore(ReadOnlyTargetRules Target) : base( Target )
  {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "ViconDataStreamSDK" });
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
    }
}
