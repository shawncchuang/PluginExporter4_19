// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "private/ViconDataStream.h"
#include "ViconDataStreamActor.generated.h"

class  FViconDataStreamManager
{
public:
  static TSharedPtr<FViconDataStream>& RegisterDataStream( const FString& StreamIDName );
  static bool UnRegisterDataStream( const FString& StreamIDName );
  DATASTREAMCORE_API static TSharedPtr<FViconDataStream> GetDataStream( const FString& i_rIDName );

private: 
  FViconDataStreamManager();
  ~FViconDataStreamManager();

  static TMap< FString, TSharedPtr<FViconDataStream> > s_DataStreamMap;
};

UCLASS( BlueprintType )
class DATASTREAMCORE_API AViconDataStreamActor : public AActor
{
  GENERATED_UCLASS_BODY()
public:
  ~AViconDataStreamActor();
  UPROPERTY( EditAnywhere, Category = Server, meta = (PinShownByDefault) )
    FString ServerName;

  UPROPERTY( EditAnywhere, Category = Server, meta = (PinShownByDefault) )
    FString Port;

  UPROPERTY( EditAnywhere, Category = Server, meta = (PinShownByDefault) )
    bool IsRetimed;

  UPROPERTY( EditAnywhere, Category = Server, meta = (EditCondition="IsRetimed") )
    float Offset;

  //Try to reconnect everytime settings are changed
#if WITH_EDITOR
  virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif

  virtual void PostLoad() override;

  bool ReConnect();

private:
  UPROPERTY()
  FString m_CurrentIdName;
  TSharedPtr<FViconDataStream> m_pDataStream;
};

