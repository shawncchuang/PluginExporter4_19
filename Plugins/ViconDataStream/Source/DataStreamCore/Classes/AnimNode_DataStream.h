#pragma once
#include "private/DataStreamToSkeletonBinding.h"
#include "Animation/AnimNodeBase.h"
#include "AnimNode_DataStream.generated.h"
DECLARE_LOG_CATEGORY_CLASS( LogDataStreamCore, Warning, All )

USTRUCT(BlueprintType)
struct  DATASTREAMCORE_API FAnimNode_DataStream : public FAnimNode_Base
{
  GENERATED_USTRUCT_BODY()

public:

  UPROPERTY( Transient )
    FPoseLink InPose;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Vicon", meta = (PinShownByDefault) )
    FString SubjectName;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Vicon", meta = (PinShownByDefault) )
    FString DataStream_ID_Name;

public:

  FAnimNode_DataStream();
  ~FAnimNode_DataStream();

  // Keep the binding here.
  FDataStreamToSkeletonBinding m_SkeletonBinding;


  // FAnimNode_Base interface
  virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
  virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
  virtual void Evaluate_AnyThread(FPoseContext& Output) override;
  virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext & Context) override;
  // End of FAnimNode_Base interface

private:
  TSharedPtr< FViconDataStream > m_pDataStream;

};
