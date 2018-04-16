#pragma once
#include "AnimNode_DataStream.h"
#include "Array.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"

// Represents a binding of an ViconStream subject to a specific skeleton.
// Resolves UE4 bone indices and stores them.

class FViconDataStream;

class FDataStreamToSkeletonBinding
{
public:

  // Default constructor.
  FDataStreamToSkeletonBinding();

  // Bind the given ViconStreamSubject to the given skeleton and store the result.
  // Since the member properties are not stored on file, this should be called
  // from runtime each time, to set up the members.
  bool BindToSkeleton( const TSharedPtr<FViconDataStream>& ViconStream, const FString& subject, USkeleton* skeleton, USkeletalMeshComponent* component );

  bool UpdatePose( TArray<FTransform>& outPose );

  bool IsBound() { return m_bBound; };

  // Access the UE4 bone index given the segment index.
  int32 GetUE4BoneIndex( int32 SegmentIndex ) const;

  // Access the Vicon segment name given the segment index.
  FString GetViconSegmentName( int32 SegmentIndex ) const;

  bool GetPoseForSubject( const FString& i_rSubjectName, TArray<FTransform>& o_rOutPose );

private:
  TSharedPtr<FViconDataStream> m_pViconStream;

  FString mSubjectName;
  // Cache the UE4 bone indices corresponding to subject segments.
  TArray<int32> UE4BoneIndices;
  // Cache Vicon Subject bone names
  TArray<FString> m_SegmentNames;

  //Flag to make sure we are bound to a valid stream
  bool m_bBound;
};

