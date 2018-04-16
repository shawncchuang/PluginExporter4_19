#include "DataStreamEditorModule.h"
#include "AnimGraphNode_DataStream.h"
#include "ModuleManager.h"


/**
* DataStreamEditor module implementation
*/

void FDataStreamEditorModule::StartupModule()
{
  
}

void FDataStreamEditorModule::ShutdownModule()
{
  
}


DEFINE_LOG_CATEGORY(LogDataStreamEditor);
IMPLEMENT_MODULE(FDataStreamEditorModule, DataStreamEditor);
