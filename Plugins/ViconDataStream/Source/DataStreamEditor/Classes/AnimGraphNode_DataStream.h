#pragma once

#include "AnimGraphNode_Base.h"
#include "AnimNode_DataStream.h"
#include "AnimGraphNode_DataStream.generated.h"

UCLASS(MinimalAPI)
class UAnimGraphNode_DataStream: public UAnimGraphNode_Base
{
  GENERATED_UCLASS_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
  FAnimNode_DataStream Node;

#if WITH_EDITOR
  // UEdGraphNode interface
  virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
  virtual FLinearColor GetNodeTitleColor() const override;
  virtual FText GetTooltipText() const override;
  virtual FString GetNodeCategory() const override;
  // End of UEdGraphNode interface
#endif
};
