#include "DataStreamToSkeletonBinding.h"
#include "ViconDataStream.h"

#include "AnimNode_DataStream.h"

// Default constructor.
FDataStreamToSkeletonBinding::FDataStreamToSkeletonBinding()
  : m_pViconStream (nullptr)
  , m_bBound( false )
{}

// Bind the given subject to the given skeleton and store the result.
bool FDataStreamToSkeletonBinding::BindToSkeleton( const TSharedPtr<FViconDataStream>& ViconStream, const FString& subject, USkeleton* skeleton, USkeletalMeshComponent* component )
{
  if ( ViconStream.Get() == nullptr )
  {
    UE_LOG( LogDataStreamCore, Error, TEXT( "Invalid Vicon Stream for skeleton binding" ) );
    return false;
  }
  unsigned int numBoneDefs = INDEX_NONE;
  if ( ViconStream->IsConnected() )
  {
    ViconStream->GetFrame();
    ViconStream->GetSegmentCountForSubject( subject, numBoneDefs );
    m_SegmentNames.Empty( numBoneDefs );
    UE4BoneIndices.Empty( numBoneDefs );
    for ( unsigned int i = 0; i < numBoneDefs; ++i )
    {
      int32 ue4BoneIndex = INDEX_NONE;
      FString Name;
      FString ParentName;
      ViconStream->GetSegmentNameForSubject( subject, i, Name );
      ViconStream->GetSegmentParentNameForSubject( subject, Name, ParentName );

      ue4BoneIndex = component->GetBoneIndex( FName( *Name ) );
      if ( ue4BoneIndex != INDEX_NONE )
      {
        // Validate the hierarchy.
        // The parent in the subject hierarchy should be the same
        // as the parent in the UE4 hierarchy.
        //const FBoneNode& boneNode = skeleton->GetBoneTree()[ue4BoneIndex];

        if ( ParentName != "" )
        {
          // Non-root bone.
          // Make sure that its parent corresponds to the UE4 parent.

          int32 parentBoneIndex = component->GetBoneIndex( FName( *ParentName ) ); //skeleton.GetBoneTreeIndex(ParentName);
          if ( parentBoneIndex != INDEX_NONE )
          {
            if ( parentBoneIndex != skeleton->GetReferenceSkeleton().GetParentIndex( ue4BoneIndex ) )
            {
              UE_LOG( LogDataStreamCore, Error, TEXT( "Subject parent joint %s for joint %s should be the parent in the UE4 skeleton hierarchy as well" ), *ParentName, *Name );
              return false;
            }
          }
          else
          {
            UE_LOG( LogDataStreamCore, Error, TEXT( "Unknown parent joint %s for joint %s" ), *ParentName, *Name );
            return false;
          }
        }
        UE4BoneIndices.Add( ue4BoneIndex );
        m_SegmentNames.Add( Name );
      }
    }
    m_bBound = true;
  }
  else
  {
    m_bBound = false;
  }
  m_pViconStream = ViconStream;
  mSubjectName = subject;
  return true;
}

static void ConvertTransformToFromMaya( FTransform& Transform, bool convertTranslation = true )
{

  FVector Translation = Transform.GetTranslation();
  FQuat Rotation = Transform.GetRotation();

  Rotation.X = -Rotation.X;
  Rotation.Z = -Rotation.Z;
  Transform.SetRotation( Rotation );

  if ( convertTranslation )
  {
    Translation.Y = -Translation.Y;
    Transform.SetTranslation( Translation );
  }
}

bool FDataStreamToSkeletonBinding::UpdatePose( TArray<FTransform>& outPose )
{
  if ( m_pViconStream.Get() == nullptr )
  {
    UE_LOG( LogDataStreamCore, Warning, TEXT( "ViconStream object is not valid" ) );
    return false;
  }

  if ( !m_bBound )
  {
    UE_LOG( LogDataStreamCore, Error, TEXT( "Skeleton not bound to a valid stream" ) );
    return false;
  }

  if ( !m_pViconStream->IsConnected() )
    return false;

  return GetPoseForSubject( mSubjectName, outPose );
}

// Access the UE4 bone index given the bone def index.
int32 FDataStreamToSkeletonBinding::GetUE4BoneIndex( int32 boneDefIndex ) const
{
  if ( UE4BoneIndices.IsValidIndex( boneDefIndex ) )
    return UE4BoneIndices[boneDefIndex];
  return -1;
}

FString FDataStreamToSkeletonBinding::GetViconSegmentName( int32 SegmentIndex ) const
{
  if ( m_SegmentNames.IsValidIndex( SegmentIndex ) )
    return m_SegmentNames[SegmentIndex];
  return "";
}


bool FDataStreamToSkeletonBinding::GetPoseForSubject( const FString& i_rSubjectName, TArray<FTransform>& o_rOutPose  )
{
  unsigned int SegmentCount( 0 );
  if (m_pViconStream->GetFrame()!= ESuccess )
  {
    return false;
  }

  if (ESuccess != m_pViconStream->GetSegmentCountForSubject( i_rSubjectName, SegmentCount ))
  {
    return false;
  }

  for (unsigned int i = 0; i < SegmentCount; ++i)
  {
    int32 UE4Index = GetUE4BoneIndex( i );
    if (UE4Index == INDEX_NONE)
    {
      UE_LOG( LogDataStreamCore, Error, TEXT( "Invalid Bone Mapping from DataStream to UE4" ) );
      return false;
    }

    FTransform Pose = o_rOutPose[UE4Index];
    m_pViconStream->GetSegmentLocalPose( i_rSubjectName, GetViconSegmentName(i), Pose );
    o_rOutPose[UE4Index] = Pose;
  }
  return true;
}

