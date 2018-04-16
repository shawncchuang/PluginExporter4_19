// Fill out your copyright notice in the Description page of Project Settings.

#include "ViconDataStreamActor.h"

TMap< FString, TSharedPtr<FViconDataStream> > FViconDataStreamManager::s_DataStreamMap = TMap< FString, TSharedPtr<FViconDataStream> >() ;

TSharedPtr<FViconDataStream>& FViconDataStreamManager::RegisterDataStream( const FString& StreamIDName )
{
  if (!s_DataStreamMap.Contains( StreamIDName ))
  {
    TSharedPtr<FViconDataStream> pDataStream = MakeShareable( new FViconDataStream() );
    s_DataStreamMap.Add( StreamIDName, pDataStream );
    return s_DataStreamMap[StreamIDName];
  }
  else
  {
    UE_LOG( LogDataStreamCore, Warning, TEXT( "Stream Existed!" ) );
    return  s_DataStreamMap[ StreamIDName];
  }
}

bool FViconDataStreamManager::UnRegisterDataStream( const FString& StreamIDName )
{
  if (s_DataStreamMap.Contains( StreamIDName ))
  {
    s_DataStreamMap.Remove( StreamIDName );
    return true;
  }
  else
  {
    return false;
    UE_LOG( LogDataStreamCore, Warning, TEXT( "Stream doesn't exist!" ) );
  }
}

TSharedPtr<FViconDataStream> FViconDataStreamManager::GetDataStream( const FString& i_rIDName )
{
  //!!!!!!!!!!!Add mutex because GetDataStream Could be called on different threads
  if (!s_DataStreamMap.Contains( i_rIDName ))
  {
    return TSharedPtr<FViconDataStream>();
  }

  return s_DataStreamMap[i_rIDName];
}

AViconDataStreamActor::AViconDataStreamActor( const class FObjectInitializer& PCIP )
  : Super( PCIP )
{
  // workaround for not able to access actor in property panel
  FString IDName;
  GetName( IDName );
  if (IDName != "Default__ViconDataStreamActor")
  {
    m_pDataStream = FViconDataStreamManager::RegisterDataStream( IDName );
    m_CurrentIdName = IDName;
    if (!ReConnect())
    {
      UE_LOG( LogDataStreamCore, Warning, TEXT( "Failed to Connect to datastream" ) );
    }
  }

}

AViconDataStreamActor::~AViconDataStreamActor()
{
  FViconDataStreamManager::UnRegisterDataStream( m_CurrentIdName );
}

#if WITH_EDITOR
void AViconDataStreamActor::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent )
{
  Super::PostEditChangeProperty(PropertyChangedEvent);
  if (PropertyChangedEvent.Property != nullptr)
  {
    const FName PropertyName( PropertyChangedEvent.Property->GetFName() );
    if (PropertyName == GET_MEMBER_NAME_CHECKED( AViconDataStreamActor, ServerName )
      || PropertyName == GET_MEMBER_NAME_CHECKED( AViconDataStreamActor, Port )
      || PropertyName == GET_MEMBER_NAME_CHECKED( AViconDataStreamActor, IsRetimed ))
    {
      if (!ReConnect())
      {
        UE_LOG( LogDataStreamCore, Warning, TEXT( "Failed to Connect to datastream" ) );
      }
    }
    else if (PropertyName == GET_MEMBER_NAME_CHECKED( AViconDataStreamActor, Offset ))
    {
      m_pDataStream->SetOffset( Offset );
    }
    else if (PropertyName == "ActorLabel")
    {
      FViconDataStreamManager::UnRegisterDataStream( m_CurrentIdName );
      FString IDName;
      GetName( IDName );
      m_pDataStream = FViconDataStreamManager::RegisterDataStream( IDName );
      m_CurrentIdName = IDName;
    }
  }
}
#endif

void AViconDataStreamActor::PostLoad()
{
  Super::PostLoad();
  ReConnect();
}


bool AViconDataStreamActor::ReConnect()
{
  m_pDataStream->Disconnect();
  FString ServerString = ServerName + ":" + Port;
  EResult ret = m_pDataStream->Connect( ServerString, IsRetimed );
  if (ret != EResult::ESuccess)
  {
    return false;
  }
  return true;
}
