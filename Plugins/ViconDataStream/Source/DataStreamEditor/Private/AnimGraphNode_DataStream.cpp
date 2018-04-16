#include "AnimGraphNode_DataStream.h"

UAnimGraphNode_DataStream::UAnimGraphNode_DataStream(const class FObjectInitializer& PCIP)
  : Super(PCIP)
{
}

FText UAnimGraphNode_DataStream::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
  return FText::FromString(FString::Printf(TEXT("Vicon DataStream")));
}

FLinearColor UAnimGraphNode_DataStream::GetNodeTitleColor() const
{
  return FLinearColor(0.75f, 0.75f, 0.75f);
}

FText UAnimGraphNode_DataStream::GetTooltipText() const
{
  return FText::FromString(TEXT("Retrieves streamed skeletal animation from Vicon DataStream Server"));
}

FString UAnimGraphNode_DataStream::GetNodeCategory() const
{
  return FString("Vicon");
}



