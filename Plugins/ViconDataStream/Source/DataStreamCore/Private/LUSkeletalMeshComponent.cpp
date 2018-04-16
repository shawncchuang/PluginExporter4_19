// Fill out your copyright notice in the Description page of Project Settings.

#include "LUSkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "PrimitiveSceneProxy.h"
#include "Misc/ScopeLock.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "PrimitiveSceneInfo.h"
#include <fstream>
#include "ViconDataStreamActor.h"

namespace {
  /** This is to prevent destruction of components while they are
  in the middle of being accessed by the render thread */
  FCriticalSection CritSect;

  /** Console variable for specifying whether motion controller late update is used */
  TAutoConsoleVariable<int32> CVarEnableViconObjectLateUpdate(
    TEXT("vr.EnableObjectLateUpdate"),
    1,
    TEXT("This command allows you to specify whether the motion controller late update is applied.\n")
    TEXT(" 0: don't use late update\n")
    TEXT(" 1: use late update (default)"),
  ECVF_Cheat);
  //static std::vector<double> timestamps;
} // anonymous namespace

ULUSkeletalMeshComponent::ULUSkeletalMeshComponent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , Offset(24.44)
{
  if (!SubjectName.IsEmpty())
  {
    m_pDataStream = FViconDataStreamManager::GetDataStream( SubjectName );
  }
}

ULUSkeletalMeshComponent::~ULUSkeletalMeshComponent()
{ 
  if (ViewExtension.IsValid())
  {
    {
      // This component could be getting accessed from the render thread so it needs to wait
      // before clearing MotionControllerComponent and allowing the destructor to continue
      FScopeLock ScopeLock(&CritSect);
      ViewExtension->LUSkeletalMeshComponent = NULL;
    }

#if  ENGINE_MAJOR_VERSION == 4  && ENGINE_MINOR_VERSION <= 17 
    if (GEngine)
    {
      GEngine->ViewExtensions.Remove(ViewExtension);
    }
#endif
  }
  ViewExtension.Reset();
  //std::ofstream test( "d:/timestamp.csv", std::ofstream::out);
  //if (timestamps.size() == 0)
  //  return;
  //for( int i=0; i< timestamps.size()-1; ++i )
  //{
  //  test << timestamps[i + 1] - timestamps[i] << "\n";
  //}
  //test.close();
}

bool ULUSkeletalMeshComponent::PollRootState(FVector& o_rPosition, FQuat& o_rOrientation)
{
  if ((m_pDataStream.Get() == nullptr) )
  {
    return false;
  }
  m_pDataStream->SetOffset( Offset );
  m_pDataStream->GetFrame();
  return m_pDataStream->GetRootPose( SubjectName, o_rPosition , o_rOrientation ) == EResult::ESuccess;
}

void ULUSkeletalMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  FVector Position;
  FQuat Orientation;
  PollRootState( Position, Orientation);
  m_Pos = Position;
  m_Rot = Orientation;

  if (!ViewExtension.IsValid() && GEngine)
  {
#if ENGINE_MAJOR_VERSION ==4  && ENGINE_MINOR_VERSION <= 17 
    TSharedPtr< FViewExtension, ESPMode::ThreadSafe > NewViewExtension(new FViewExtension(this));
    ViewExtension = NewViewExtension;
    GEngine->ViewExtensions.Add(ViewExtension);

#else
    TSharedPtr< FViewExtension, ESPMode::ThreadSafe > NewViewExtension = FSceneViewExtensions::NewExtension<FViewExtension>(this);
    ViewExtension = NewViewExtension;
#endif
  }
}

#if WITH_EDITOR
void ULUSkeletalMeshComponent::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
  if (PropertyChangedEvent.Property != nullptr)
  {
    const FName PropertyName( PropertyChangedEvent.Property->GetFName() );
    if (PropertyName == GET_MEMBER_NAME_CHECKED( ULUSkeletalMeshComponent, SubjectName ))
    {
      if (!SubjectName.IsEmpty())
      {
        m_pDataStream = FViconDataStreamManager::GetDataStream( SubjectName );
      }
    }
  }
}
#endif

//=============================================================================

void ULUSkeletalMeshComponent::FViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
  if (!LUSkeletalMeshComponent)
  {
    return;
  }

  FScopeLock ScopeLock(&CritSect);
  if (!LUSkeletalMeshComponent || !CVarEnableViconObjectLateUpdate.GetValueOnGameThread() )
  {
    return;
  }
  LateUpdatePrimitives.Reset();
  GatherLateUpdatePrimitives( LUSkeletalMeshComponent, LateUpdatePrimitives);
}

void ULUSkeletalMeshComponent::FViewExtension::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
  if (!LUSkeletalMeshComponent)
  {
    return;
  }

  FScopeLock ScopeLock(&CritSect);

  if (!LUSkeletalMeshComponent || !CVarEnableViconObjectLateUpdate.GetValueOnRenderThread() )
  {
    return;
  }
  //timestamps.emplace_back( FPlatformTime::Seconds() );
  // Poll state for the most recent controller transform
  FVector Position;
  FQuat Orientation(0,0,0,1);
  if (LUSkeletalMeshComponent->PollRootState( Position, Orientation ))
  {
    Orientation.Normalize();
    if (LateUpdatePrimitives.Num())
    {
      // Apply delta to the affected scene proxies
      for (auto PrimitiveInfo : LateUpdatePrimitives)
      {
        FPrimitiveSceneInfo* RetrievedSceneInfo = InViewFamily.Scene->GetPrimitiveSceneInfo( *PrimitiveInfo.IndexAddress );
        FPrimitiveSceneInfo* CachedSceneInfo = PrimitiveInfo.SceneInfo;
        // If the retrieved scene info is different than our cached scene info then the primitive was removed from the scene
        if (CachedSceneInfo == RetrievedSceneInfo && CachedSceneInfo->Proxy)
        {
          FQuat Rot = LUSkeletalMeshComponent->m_Rot;
          Rot.Normalize();
          const FTransform OldLocalToWorldTransform = FTransform( Rot, LUSkeletalMeshComponent->m_Pos );
          const FTransform NewLocalToWorldTransform = FTransform( Orientation, Position, LUSkeletalMeshComponent->GetComponentScale() );
          const FMatrix LateUpdateTransform = (OldLocalToWorldTransform.Inverse() * NewLocalToWorldTransform).ToMatrixWithScale();

          CachedSceneInfo->Proxy->ApplyLateUpdateTransform( LateUpdateTransform );
        }
      }
      LateUpdatePrimitives.Reset();
    }
  }
}

void ULUSkeletalMeshComponent::FViewExtension::GatherLateUpdatePrimitives(USceneComponent* Component, TArray<LateUpdatePrimitiveInfo>& Primitives)
{
  // If a scene proxy is present, cache it
  UPrimitiveComponent* PrimitiveComponent = dynamic_cast<UPrimitiveComponent*>(Component);
  if (PrimitiveComponent && PrimitiveComponent->SceneProxy)
  {
    FPrimitiveSceneInfo* PrimitiveSceneInfo = PrimitiveComponent->SceneProxy->GetPrimitiveSceneInfo();
    if (PrimitiveSceneInfo)
    {
      LateUpdatePrimitiveInfo PrimitiveInfo;
      PrimitiveInfo.IndexAddress = PrimitiveSceneInfo->GetIndexAddress();
      PrimitiveInfo.SceneInfo = PrimitiveSceneInfo;
      Primitives.Add(PrimitiveInfo);
    }
  }

  // Gather children proxies
  const int32 ChildCount = Component->GetNumChildrenComponents();
  for (int32 ChildIndex = 0; ChildIndex < ChildCount; ++ChildIndex)
  {
    USceneComponent* ChildComponent = Component->GetChildComponent(ChildIndex);
    if (!ChildComponent)
    {
      continue;
    }

    GatherLateUpdatePrimitives(ChildComponent, Primitives);
  }
}

