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
ViconDataStreamSDK::CPP::Output_GetSubjectRootSegmentName ReRootSegmentName;

ViconDataStreamSDK::CPP::Output_GetSegmentLocalTranslation ReSegmentLocalTrasnslation;
ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationEulerXYZ ReSegmentLocalRotationEuler;
ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationQuaternion ReSegmentLocalRotationQuaternion;

ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationEulerXYZ ReSegmentGlobalRotationEuler;


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

		FString _Translation = "";
		for  (double &_value : ReMarkerTranslation.Translation)
		{
			_Translation += "," + FString::SanitizeFloat(_value);
		} 

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GetMarkerGlobalTranslation : %s"), *_Translation));
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
		
		UE_LOG(LogTemp, Warning, TEXT("Test :DataStream_SegmentCount : %d"), SegmentCount);
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Segment Count : %d"),SegmentCount));
	}

}

void UViconClient::DataStream_GetSubjectRootSegmentName(FString SubjectName, FString &SegmentName)
{
	ReRootSegmentName = MyClient.GetSubjectRootSegmentName(TCHAR_TO_UTF8(*SubjectName));

	if (ReRootSegmentName.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{
		std::string _rootSegmentName = ReRootSegmentName.SegmentName;
		SegmentName = _rootSegmentName.c_str();
	}

}

void UViconClient::DataStream_GetSegmentLocalTranslation(FString SubjectName, FString SegmentName, FVector &NewLocation)
{
	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	ReSegmentLocalTrasnslation = MyClient.GetSegmentLocalTranslation(_SubjectName, _SegmentName);
 
	bool ErrorLog = true;
	FString   result = "Segment Local Translation :";
	switch (ReSegmentLocalTrasnslation.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success :
		ErrorLog = false;

		NewLocation.X = ReSegmentLocalTrasnslation.Translation[0];
		NewLocation.Y= ReSegmentLocalTrasnslation.Translation[1];
		NewLocation.Z = ReSegmentLocalTrasnslation.Translation[2];

		/*
		for (double &value : ReSegmentLocalTrasnslation.Translation)
		{
			result += ", "+FString::SanitizeFloat(value);
		}
		*/

		break;
	case ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	case ViconDataStreamSDK::CPP::Result::NoFrame:
		result += "NoFrame";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
		result += "InvalidSubjectName";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSegmentName:
		result += "InvalidSegmentName";
		break;
 
	}
	if (ErrorLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
	}
	

}

void UViconClient::DataStream_GetSegmentLocalRotationEuler(FString SubjectName, FString SegmentName, FVector &NewRotation)
{
	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	ReSegmentLocalRotationEuler = MyClient.GetSegmentLocalRotationEulerXYZ(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Local Rotation_Euler :";

	switch (ReSegmentLocalRotationEuler.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewRotation.X = ReSegmentLocalRotationEuler.Rotation[0];
		NewRotation.Y = ReSegmentLocalRotationEuler.Rotation[1];
		NewRotation.Z = ReSegmentLocalRotationEuler.Rotation[2];

		
		for (double &value : ReSegmentLocalRotationEuler.Rotation)
		{
		result += ", "+FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);

		break;
	case ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	case ViconDataStreamSDK::CPP::Result::NoFrame:
		result += "NoFrame";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
		result += "InvalidSubjectName";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSegmentName:
		result += "InvalidSegmentName";
		break;

	}
	if (ErrorLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
	}

}

void UViconClient::DataStream_GetSegmentLocalRotationQuaternion(FString SubjectName, FString SegmentName, FVector &NewRotation)
{
	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	ReSegmentLocalRotationQuaternion = MyClient.GetSegmentLocalRotationQuaternion(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Local Rotation_Quaternion :";

	switch (ReSegmentLocalRotationQuaternion.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewRotation.X = ReSegmentLocalRotationQuaternion.Rotation[0];
		NewRotation.Y = ReSegmentLocalRotationQuaternion.Rotation[1];
		NewRotation.Z = ReSegmentLocalRotationQuaternion.Rotation[2];


		for (double &value : ReSegmentLocalRotationQuaternion.Rotation)
		{
			result += ", " + FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);

		break;
	case ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	case ViconDataStreamSDK::CPP::Result::NoFrame:
		result += "NoFrame";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
		result += "InvalidSubjectName";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSegmentName:
		result += "InvalidSegmentName";
		break;

	}
	if (ErrorLog)
	{
		 
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
	}


}

void UViconClient::DataStream_GetSegmentGlobalRotationEulerXYZ(FString SubjectName, FString SegmentName, FVector  &NewRotation)
{

	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	ReSegmentGlobalRotationEuler = MyClient.GetSegmentGlobalRotationEulerXYZ(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Global Rotation_Euler :";

	switch (ReSegmentGlobalRotationEuler.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewRotation.X = ReSegmentGlobalRotationEuler.Rotation[0];
		NewRotation.Y = ReSegmentGlobalRotationEuler.Rotation[1];
		NewRotation.Z = ReSegmentGlobalRotationEuler.Rotation[2];


		for (double &value : ReSegmentGlobalRotationEuler.Rotation)
		{
			result += ", " + FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);

		break;
	case ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	case ViconDataStreamSDK::CPP::Result::NoFrame:
		result += "NoFrame";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
		result += "InvalidSubjectName";
		break;
	case ViconDataStreamSDK::CPP::Result::InvalidSegmentName:
		result += "InvalidSegmentName";
		break;

	}
	if (ErrorLog)
	{

		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
	}

}

void UViconClient::DebugMessage(FString Message)
{

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *Message));
}



 
 