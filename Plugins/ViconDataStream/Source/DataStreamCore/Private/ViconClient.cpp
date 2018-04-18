// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ViconClient.h"
 

ViconDataStreamSDK::CPP::Client MyClient;
ViconDataStreamSDK::CPP::Output_GetVersion CurrentVersion;
ViconDataStreamSDK::CPP::Output_IsConnected ReIsConnected;
ViconDataStreamSDK::CPP::Output_GetSubjectName ReSubjectName;
ViconDataStreamSDK::CPP::Output_GetSubjectCount ReSubjectCount;
ViconDataStreamSDK::CPP::Output_GetMarkerCount ReMarkerCount;
ViconDataStreamSDK::CPP::Output_GetMarkerGlobalTranslation ReMarkerTranslation;
ViconDataStreamSDK::CPP::Output_GetSegmentCount ReSegmentCount;
ViconDataStreamSDK::CPP::Output_GetLabeledMarkerCount ReLabeledMarkerCount;

 

FString ServerAddress;
bool IsConnected;
int32  SegmentCount = 0;
int32  SubjectCount = 0;
int32 MakerCount = 0;
FString SubjectName;


void UViconClient::DataStream_GetSDKVersion()
{

	
	CurrentVersion = MyClient.GetVersion();
	float  _CurrentVersionMajor = CurrentVersion.Major;
	float _CurrentVersionPoint = CurrentVersion.Point;
	float _CurrentVersion = _CurrentVersionMajor + _CurrentVersionPoint;
	//UE_LOG(LogTemp, Warning, TEXT("Test :ViconDataStramSDK Version: %f"), _CurrentVersion);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ViconDataStramSDK Version : %f"), _CurrentVersion));
}

void UViconClient::DataStream_Connect(FString ServerName, bool &IsConnected)
{
	ServerAddress = ServerName;
	MyClient.Connect(TCHAR_TO_UTF8(*ServerName));
	ReIsConnected = MyClient.IsConnected();
	IsConnected = ReIsConnected.Connected;
	MyClient.EnableSegmentData();
	MyClient.EnableMarkerData();

}
void UViconClient::DataStream_CheckConnected(bool &IsConnected)
{
	//UE_LOG(LogTemp, Warning, TEXT("ViconDataStram Connected: %s"), *ServerAddress);
	//MyClient.Connect(TCHAR_TO_UTF8(*ServerAddress));
	ReIsConnected = MyClient.IsConnected();
	IsConnected = ReIsConnected.Connected;
	 
}
void UViconClient::DataStream_GetSubjectCount(int32 &SubjectCount)
{
	MyClient.GetFrame();
	ReSubjectCount = MyClient.GetSubjectCount();
	if (ReSubjectCount.Result)
	{
		SubjectCount = ReSubjectCount.SubjectCount;
	}

}

void UViconClient::DataStream_GetSubjectName(FString &SubjectName)
{
	ReSubjectName = MyClient.GetSubjectName(0);
	if (ReSubjectName.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{
		std::string SubjectNameStr =  ReSubjectName.SubjectName;
	    SubjectName = SubjectNameStr.c_str();
	}
}

void UViconClient::DataStream_GetMarkerCount(FString SubjectName, int32 &MakerCount)
{
	MyClient.GetFrame();
	ReMarkerCount = MyClient.GetMarkerCount(TCHAR_TO_UTF8(*SubjectName));
	if (ReMarkerCount.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{
		MakerCount = ReMarkerCount.MarkerCount;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Maker Count : %d"), MakerCount));
	}
}

void UViconClient::DataStream_GetMakerGolbalTranslation(FString SubjectName, FString MarkerName, FVector &NewLocation)
{
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _MarkerName = TCHAR_TO_UTF8(*MarkerName);
	MyClient.GetFrame();
	ReMarkerTranslation = MyClient.GetMarkerGlobalTranslation(_SubjectName, _MarkerName);
	if (ReMarkerTranslation.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{

		 
		NewLocation.X = ReMarkerTranslation.Translation[0];
		NewLocation.Y = ReMarkerTranslation.Translation[1];
		NewLocation.Z = ReMarkerTranslation.Translation[2];

		
		double  x = ReMarkerTranslation.Translation[0];
		double y = ReMarkerTranslation.Translation[1];
		double z = ReMarkerTranslation.Translation[2];
 
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GetMarkerGlobalTranslation : x:%d, y:%d, z:%d"), x,y,z));
	}
	else if (ReMarkerTranslation.Result == ViconDataStreamSDK::CPP::Result::Enum::NoFrame)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GetMarkerGlobalTranslation: No Frame"));
	}
	else if (ReMarkerTranslation.Result == ViconDataStreamSDK::CPP::Result::Enum::NotConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GetMarkerGlobalTranslation:  NotConnected"));
	}
	 
}

void UViconClient::DataStream_GetLabelMarkerCount()
{

	ReLabeledMarkerCount = MyClient.GetLabeledMarkerCount();
	if (ReLabeledMarkerCount.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{
		int32 LabeledMarkerCount = ReLabeledMarkerCount.MarkerCount;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Labeled Marker Count : %d"), LabeledMarkerCount));
	}


}


void UViconClient::DataStream_SegmentCount(FString SubjectName)
{

	ReSegmentCount = MyClient.GetSegmentCount(TCHAR_TO_UTF8(*SubjectName));
	if (ReSegmentCount.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{
	 
		SegmentCount = ReSegmentCount.SegmentCount;
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Segment Count : %d"),SegmentCount));
	}

}

