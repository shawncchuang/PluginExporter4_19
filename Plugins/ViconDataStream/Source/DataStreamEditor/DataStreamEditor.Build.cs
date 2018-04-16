using UnrealBuildTool;

public class DataStreamEditor : ModuleRules
{
  public DataStreamEditor(ReadOnlyTargetRules Target) : base( Target )
  {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "DataStreamCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "AnimGraph", "BlueprintGraph" });
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
    }
}
