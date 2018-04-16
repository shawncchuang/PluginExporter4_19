#include "ViconDataStream.h"
#include <numeric>

namespace
{
  void MatrixToQuaternion( const double i_M[9], double( &o_Q )[4] )
  {
    double Trace = i_M[0 * 3 + 0] + i_M[1 * 3 + 1] + i_M[2 * 3 + 2];
    double Root;
    if (Trace > 0)
    {
      Root = std::sqrt( Trace + 1.0 );  // 2w
      o_Q[3] = 0.5*Root;
      Root = 0.5 / Root;  // 1/(4w)
      o_Q[0] = (i_M[2 * 3 + 1] - i_M[1 * 3 + 2])*Root;
      o_Q[1] = (i_M[0 * 3 + 2] - i_M[2 * 3 + 0])*Root;
      o_Q[2] = (i_M[1 * 3 + 0] - i_M[0 * 3 + 1])*Root;
    }
    else
    {
      int s_Next[3] = { 1, 2, 0 };
      int i = 0;
      if (i_M[1 * 3 + 1] > i_M[0 * 3 + 0])
        i = 1;
      if (i_M[2 * 3 + 2] > i_M[i * 3 + i])
        i = 2;
      int j = s_Next[i];
      int k = s_Next[j];
      Root = std::sqrt( i_M[i * 3 + i] - i_M[j * 3 + j] - i_M[k * 3 + k] + 1.0 );
      o_Q[i] = 0.5*Root;
      Root = 0.5 / Root;
      o_Q[3] = (i_M[k * 3 + j] - i_M[j * 3 + k])*Root;
      o_Q[j] = (i_M[j * 3 + i] + i_M[i * 3 + j])*Root;
      o_Q[k] = (i_M[k * 3 + i] + i_M[i * 3 + k])*Root;
    }
    const double InnerProduct = std::inner_product( o_Q, o_Q + 4, o_Q, 0.0 );
    const double Magnitude = std::sqrt( InnerProduct );
    o_Q[0] /= Magnitude;
    o_Q[1] /= Magnitude;
    o_Q[2] /= Magnitude;
    o_Q[3] /= Magnitude;
  }
}

FViconDataStream::FViconDataStream()
  : m_bRetimed (false)
  , m_Offset( 0.0 )
{
  m_pClient = &m_Client;
}

FViconDataStream::~FViconDataStream()
{
  Disconnect();
}

EResult FViconDataStream::Connect(const FString& i_rServer, bool i_bRetimed )
{
  m_ServerIP = i_rServer;
  m_bRetimed = i_bRetimed;
  ViconDataStreamSDK::CPP::Output_Connect Result;
  if( i_bRetimed )
  {
    m_pClient = &m_RetimingClient;
    Result = m_RetimingClient.Connect( TCHAR_TO_UTF8(*i_rServer) );
    if( Result.Result == ViconDataStreamSDK::CPP::Result::Success || ViconDataStreamSDK::CPP::Result::ClientAlreadyConnected )
    { 
      return EResult::ESuccess;
    }
  }
  else
  {
    m_pClient = &m_Client;
    Result = m_Client.Connect( TCHAR_TO_UTF8(*i_rServer) );
    if( Result.Result == ViconDataStreamSDK::CPP::Result::Success || ViconDataStreamSDK::CPP::Result::ClientAlreadyConnected )
    {
      m_Client.EnableSegmentData();
      m_Client.EnableMarkerData();

      if( m_Client.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush ).Result == ViconDataStreamSDK::CPP::Result::Success )
      {
        return EResult::ESuccess;
      }
    }
  }

  return EResult::EError;
}

bool FViconDataStream::IsConnected()
{
  const ViconDataStreamSDK::CPP::Output_IsConnected& Result = m_pClient->IsConnected();
  return Result.Connected;
}

void FViconDataStream::Disconnect()
{
  if( m_pClient->IsConnected().Connected )
  {
    m_pClient->Disconnect();
  }
}

EResult FViconDataStream::Reconnect()
{
  return Connect( m_ServerIP, m_bRetimed );
}

EResult FViconDataStream::GetFrame()
{
  if ( m_bRetimed )
  {
    auto Result = m_RetimingClient.UpdateFrame( m_Offset);
    if (Result.Result == ViconDataStreamSDK::CPP::Result::Success)
      return EResult::ESuccess;
    return EResult::EError;
  }
  else
  { 
    const ViconDataStreamSDK::CPP::Output_GetFrame& Result = m_Client.GetFrame();

    if( Result.Result == ViconDataStreamSDK::CPP::Result::Success )
      return EResult::ESuccess;
    return EResult::EError;
  }
} 

EResult FViconDataStream::SetOffset( float Offset )
{
  if (m_bRetimed)
  {
    m_Offset = Offset;
    return ESuccess;
  }
  else
  {
    return EError;
  }
}

EResult FViconDataStream::GetSubjectCount(int& o_rCount) const
{
  const ViconDataStreamSDK::CPP::Output_GetSubjectCount& Result = m_pClient->GetSubjectCount();
  if( Result.Result == ViconDataStreamSDK::CPP::Result::Success )
  {
    o_rCount = Result.SubjectCount;
  }
  return EResult::EError;
}

EResult FViconDataStream::GetSubjectName(int Index, FString & o_rName) const
{
  const ViconDataStreamSDK::CPP::Output_GetSubjectName& Result = m_pClient->GetSubjectName( Index );
  if( Result.Result == ViconDataStreamSDK::CPP::Result::Success )
  {
    o_rName = std::string( Result.SubjectName ).c_str();
    return EResult::ESuccess;
  }
  return EResult::EError;
}

EResult FViconDataStream::GetSegmentCountForSubject(const FString& i_rSubjectNme, unsigned int& o_rCount) const
{
  o_rCount = 0;
  const ViconDataStreamSDK::CPP::Output_GetSegmentCount& Result = m_pClient->GetSegmentCount(TCHAR_TO_UTF8(*i_rSubjectNme));
  if (Result.Result == ViconDataStreamSDK::CPP::Result::Success)
  {
    o_rCount = Result.SegmentCount;
    return EResult::ESuccess;
  }

  if (Result.Result == ViconDataStreamSDK::CPP::Result::NoFrame)
  {
    UE_LOG(LogDataStreamCore, Error, TEXT("NoFrame"));
  }
  else if (Result.Result == ViconDataStreamSDK::CPP::Result::InvalidSubjectName)
  {
    UE_LOG(LogDataStreamCore, Error, TEXT("Invalid Subject Name"));
  }
  return EResult::EError;
}

EResult FViconDataStream::GetSegmentNameForSubject(const FString& i_rSubjectNme, int Index, FString& o_rSegName) const
{
   const ViconDataStreamSDK::CPP::Output_GetSegmentName& Result = m_pClient->GetSegmentName(TCHAR_TO_UTF8(*i_rSubjectNme), Index);
  
  if (Result.Result == ViconDataStreamSDK::CPP::Result::Success)
  {
    o_rSegName = std::string(Result.SegmentName).c_str();
    return EResult::ESuccess;
  }

  if (Result.Result ==ViconDataStreamSDK::CPP::Result::InvalidSubjectName)
  {
    UE_LOG(LogDataStreamCore, Error, TEXT("Invalid Subject name"));
  }
  return EResult::EError;
}

EResult FViconDataStream::GetSegmentParentNameForSubject(const FString& i_rSubjectName, const FString& i_rSegName, FString& o_rSegName) const
{
  ViconDataStreamSDK::CPP::String SubjectName(TCHAR_TO_UTF8(*i_rSubjectName));
  ViconDataStreamSDK::CPP::String SegmentName(TCHAR_TO_UTF8(*i_rSegName));

  const ViconDataStreamSDK::CPP::Output_GetSegmentParentName& Result = m_pClient->GetSegmentParentName( SubjectName, SegmentName );

  if (Result.Result == ViconDataStreamSDK::CPP::Result::Success)
  {
    o_rSegName = std::string(Result.SegmentName).c_str();
    return EResult::ESuccess;
  }
   
  if (Result.Result == ViconDataStreamSDK::CPP::Result::InvalidSubjectName)
  {
    UE_LOG(LogDataStreamCore, Error, TEXT("Invalid Subject name"));
  }

  return EResult::EError;
}

#pragma optimize( "", off)
EResult FViconDataStream::GetSegmentLocalPose( const FString& i_rSubjectName, const FString& i_rSegmentName, FTransform& o_rPose  )
{
  ViconDataStreamSDK::CPP::String SubjectName( TCHAR_TO_UTF8( *i_rSubjectName ) );
  ViconDataStreamSDK::CPP::String SegmentName( TCHAR_TO_UTF8( *i_rSegmentName ) );

  //Translation
  const ViconDataStreamSDK::CPP::Output_GetSegmentLocalTranslation& SegLocalTranslation = m_pClient->GetSegmentLocalTranslation( SubjectName, SegmentName );
  if (SegLocalTranslation.Result != ViconDataStreamSDK::CPP::Result::Success || SegLocalTranslation.Occluded)
  {
    //UE_LOG( LogDataStreamCore, Warning, TEXT( "Failed to get translation" ) );
    o_rPose.SetTranslation( FVector( 0, 0, 0 ) );
    //return EError;
  }
  else
  {
    o_rPose.SetTranslation( FVector( SegLocalTranslation.Translation[0], -SegLocalTranslation.Translation[1], SegLocalTranslation.Translation[2] )*0.1 );
  }

  //Rotation
  const ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationQuaternion& SegLocalRotation = m_pClient->GetSegmentLocalRotationQuaternion( SubjectName, SegmentName );
  if (SegLocalRotation.Result != ViconDataStreamSDK::CPP::Result::Success || SegLocalRotation.Occluded)
  {
    //return EError;
  }
  else
  {
    o_rPose.SetRotation( FQuat( -SegLocalRotation.Rotation[0], SegLocalRotation.Rotation[1], -SegLocalRotation.Rotation[2], SegLocalRotation.Rotation[3] ) );
  }

  return ESuccess;
}
#pragma optimize( "", on)

EResult FViconDataStream::GetRootPose( const FString& i_rSubjectName, FVector& o_rPosition, FQuat& o_rOrientation )
{
  ViconDataStreamSDK::CPP::String SubjectName( TCHAR_TO_UTF8( *i_rSubjectName ) );
  auto NameResult = m_pClient->GetSubjectRootSegmentName( SubjectName );

  ViconDataStreamSDK::CPP::String RootName = NameResult.SegmentName;
  if (NameResult.Result != ViconDataStreamSDK::CPP::Result::Success)
  {
    return EResult::EError;
  }
  FTransform Pose;

  EResult  Result = GetSegmentLocalPose( i_rSubjectName, std::string(RootName).c_str(), Pose );

  o_rPosition = Pose.GetTranslation();
  o_rOrientation = Pose.GetRotation();
  return Result;
}
