#pragma once

#define PCPP CPP
#undef CPP


#include <DataStreamRetimingClient.h>
#include <DataStreamClient.h>
#include <IDataStreamClientBase.h>

enum EResult
{
  ESuccess,
  EError,
};

class  FViconDataStream
{
public:
  FViconDataStream();
  ~FViconDataStream();

  EResult Connect(const FString& i_rServer, bool i_bRetimed);
  bool IsConnected();
  void Disconnect();

  EResult Reconnect();
  DATASTREAMCORE_API EResult GetFrame();
  EResult SetOffset( float Offset );

  EResult GetSubjectCount(int& o_rCount) const;
  EResult GetSubjectName(int Index, FString & o_rName) const;

  EResult GetSegmentCountForSubject(const FString& i_rSubjectNme, unsigned int& o_rCount) const;
  EResult GetSegmentNameForSubject(const FString& i_rSubjectNme, int Index, FString& o_rSegName) const;
  EResult GetSegmentParentNameForSubject(const FString& i_rSubjectName, const FString& i_rSegName, FString& o_rSegName) const;

  EResult GetSegmentLocalPose( const FString& i_rSubjectName, const FString& i_rSegmentName, FTransform& o_rPose );

  DATASTREAMCORE_API EResult GetRootPose( const FString& i_rSubjectName, FVector& o_rPosition, FQuat& o_rOrientation );

  bool IsRetimed() { return m_bRetimed; }

private:
  FString m_ServerIP;
  float m_Offset;
  
  ViconDataStreamSDK::CPP::IDataStreamClientBase*  m_pClient;
  bool m_bRetimed;

  ViconDataStreamSDK::CPP::Client m_Client;
  ViconDataStreamSDK::CPP::RetimingClient m_RetimingClient;
};


#define CPP PCPP
