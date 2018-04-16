// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/PrimitiveComponent.h"
#include "SceneViewExtension.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Launch/Resources/Version.h"
#include "LUSkeletalMeshComponent.generated.h"

/**
 * 
 */
class FViconDataStream;
UCLASS( meta = (BlueprintSpawnableComponent) )
class DATASTREAMCORE_API ULUSkeletalMeshComponent : public USkeletalMeshComponent
{
    GENERATED_UCLASS_BODY()
  ~ULUSkeletalMeshComponent();

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "LateUpdateSkeleton" )
  FString SubjectName;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "LateUpdateSkeleton" )
  FString DataStreamName;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "LateUpdateSkeleton", AdvancedDisplay )
  float Offset;


  void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

#if WITH_EDITOR
  virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif

protected:
   bool PollRootState( FVector& o_rPosition, FQuat& o_rOrientation );

private:
    TSharedPtr<FViconDataStream> m_pDataStream;

    /** Whether or not this component has authority within the frame*/
    bool bHasAuthority;

  /** View extension object that can persist on the render thread without the motion controller component */
#if  ENGINE_MAJOR_VERSION == 4  && ENGINE_MINOR_VERSION <= 17 
  class FViewExtension : public ISceneViewExtension, public TSharedFromThis<FViewExtension, ESPMode::ThreadSafe>
#else
  class FViewExtension : public FSceneViewExtensionBase
#endif
  {
  public:
#if  ENGINE_MAJOR_VERSION == 4  && ENGINE_MINOR_VERSION <= 17 
    FViewExtension( ULUSkeletalMeshComponent* i_pLUSkeletalMeshComponent ) { LUSkeletalMeshComponent = i_pLUSkeletalMeshComponent; }
#else  ENGINE_MAJOR_VERSION == 4  && ENGINE_MINOR_VERSION <= 17 
    FViewExtension( const FAutoRegister& AutoRegister, ULUSkeletalMeshComponent* i_pLUSkeletalMeshComponent )
      : FSceneViewExtensionBase( AutoRegister )
      , LUSkeletalMeshComponent( i_pLUSkeletalMeshComponent)
    {}
#endif
    virtual ~FViewExtension() {}

    /** ISceneViewExtension interface */
    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
    virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override {}
    virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
    virtual int32 GetPriority() const override { return -10; }

  private:
    friend class ULUSkeletalMeshComponent;

    /** Motion controller component associated with this view extension */
    ULUSkeletalMeshComponent* LUSkeletalMeshComponent;

    /*
    *    Late update primitive info for accessing valid scene proxy info. From the time the info is gathered
    *  to the time it is later accessed the render proxy can be deleted. To ensure we only access a proxy that is
    *  still valid we cache the primitive's scene info AND a pointer to it's own cached index. If the primitive
    *  is deleted or removed from the scene then attempting to access it via it's index will result in a different
    *  scene info than the cached scene info.
    */
    struct LateUpdatePrimitiveInfo
    {
      const int32*            IndexAddress;
      FPrimitiveSceneInfo*    SceneInfo;
    };

    /** Walks the component hierarchy gathering scene proxies */
    void GatherLateUpdatePrimitives(USceneComponent* Component, TArray<LateUpdatePrimitiveInfo>& Primitives);

    /** Primitives that need late update before rendering */
    TArray<LateUpdatePrimitiveInfo> LateUpdatePrimitives;
  };

  TSharedPtr< FViewExtension, ESPMode::ThreadSafe > ViewExtension;
  FVector m_Pos;
  FQuat m_Rot;
};

