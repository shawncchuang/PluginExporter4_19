
#include "AnimNode_DataStream.h"
#include "Animation/AnimInstanceProxy.h"
#include "ViconDataStreamActor.h"

FAnimNode_DataStream::FAnimNode_DataStream()
{
}

FAnimNode_DataStream::~FAnimNode_DataStream()
{
}

void FAnimNode_DataStream::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
  // Forward to the incoming pose link.
  check( Context.AnimInstanceProxy != nullptr );

  m_pDataStream = FViconDataStreamManager::GetDataStream( DataStream_ID_Name );
  if (m_pDataStream.Get() == nullptr)
  {
    return ;
  }

  if (!m_pDataStream->IsConnected())
  {
    m_pDataStream->Reconnect();
  }

  if (m_pDataStream.Get() == nullptr)
  {
    UE_LOG( LogDataStreamCore, Error, TEXT( "No ViconDataStream!" ) );
    InPose.Initialize( Context );
    return;
  }


  USkeleton* skeleton = Context.AnimInstanceProxy->GetSkeleton();
  if ( skeleton != nullptr )
  {
    m_SkeletonBinding.BindToSkeleton( m_pDataStream, SubjectName, skeleton, Context.AnimInstanceProxy->GetSkelMeshComponent() );
    InPose.Initialize( Context );
  }
}

void FAnimNode_DataStream::Update_AnyThread(const FAnimationUpdateContext& Context)
{
  if ( !m_SkeletonBinding.IsBound() )
  {

    USkeleton* skeleton = Context.AnimInstanceProxy->GetSkeleton();
    if ( skeleton != nullptr )
    {
      m_SkeletonBinding.BindToSkeleton( m_pDataStream, SubjectName, skeleton, Context.AnimInstanceProxy->GetSkelMeshComponent() );
    }
  }

  InPose.Update( Context );
  EvaluateGraphExposedInputs.Execute( Context );
}

void FAnimNode_DataStream::Evaluate_AnyThread(FPoseContext& Output)
{
  check( Output.AnimInstanceProxy->GetSkeleton() != nullptr );
  InPose.Evaluate( Output );

  TArray<FTransform> TempBones = Output.Pose.GetBoneContainer().GetRefPoseArray();
  if ( !m_SkeletonBinding.UpdatePose( TempBones) )
  {
    TempBones = Output.AnimInstanceProxy->GetSkeleton()->GetRefLocalPoses();
  }
  Output.Pose.CopyBonesFrom( TempBones );

  return;
}

void FAnimNode_DataStream::CacheBones_AnyThread(const FAnimationCacheBonesContext & Context)
{
  InPose.CacheBones( Context );
}

