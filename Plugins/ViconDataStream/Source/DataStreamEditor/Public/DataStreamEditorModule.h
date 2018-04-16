#pragma once

#include "ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDataStreamEditor, Warning, All);

class FDataStreamEditorModule : public IModuleInterface
{
public:
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
