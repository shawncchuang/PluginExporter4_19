// Fill out your copyright notice in the Description page of Project Settings.

#include "ViconRetimingClient.h"

ViconDataStreamSDK::CPP::RetimingClient MyRetimingClient;
ViconDataStreamSDK::CPP::Output_GetVersion ReCurrentVersion;
ViconDataStreamSDK::CPP::Output_Connect RetimingConnect;
ViconDataStreamSDK::CPP::Output_IsConnected RetimingIsConnected;
ViconDataStreamSDK::CPP::Output_UpdateFrame RetimingUpdateFrame;
ViconDataStreamSDK::CPP::Output_GetSubjectCount RetimingSubjectCount;
ViconDataStreamSDK::CPP::Output_GetSubjectName RetimingSubjectName;
ViconDataStreamSDK::CPP::Output_GetSegmentCount RetimingSegmentCount;
ViconDataStreamSDK::CPP::Output_GetSegmentName RetimingSegmentName;
ViconDataStreamSDK::CPP::Output_GetSegmentLocalTranslation RetimingSegmentLocalTrasnslation;
ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationQuaternion RetimingSegmentLocalRotationQuaternion;

float      FrameOffset;
FString RC_ServerAddress;
int32     RC_SegmentCount;
FString RC_SubjectName;
FString RC_SegmentName;

int32     RC_PitchOffsets = 0;
int32     RC_XYOffsets = -1;

void UViconRetimingClient::DataStream_GetSDKVersion(FString &SDKVersion)
{
	ReCurrentVersion = MyRetimingClient.GetVersion();
	int32  _CurrentVersionMajor = ReCurrentVersion.Major;
	int32 _CurrentVersionPoint = ReCurrentVersion.Point;
	SDKVersion = FString::FromInt(_CurrentVersionMajor) + "." + FString::FromInt(_CurrentVersionPoint);
	 
	//UE_LOG(LogTemp, Warning, TEXT("Test :ViconDataStramSDK Version: %f"), _CurrentVersion);
}
 
void UViconRetimingClient::DataStream_Connect(FString ServerName, float Offset)
{
	FrameOffset = Offset;
	RC_ServerAddress = ServerName;
	RetimingConnect = MyRetimingClient.Connect(TCHAR_TO_UTF8(*ServerName));
	//MyRetimingClient.SetStreamMode(ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch);
	RetimingUpdateFrame = MyRetimingClient.UpdateFrame(Offset);
	bool ErrorLog = true;
	FString   result = "Call Connect :";

	switch (RetimingUpdateFrame.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;
		result += "Success";
		break;
	case ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	 
	}
	if (ErrorLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
		return;
	}

}

void UViconRetimingClient::DataStream_CheckConnected(bool & IsConnected)
{

	//UE_LOG(LogTemp, Warning, TEXT("ViconDataStram Connected: %s"), *ServerAddress);
	RetimingIsConnected = MyRetimingClient.IsConnected();
	IsConnected = RetimingIsConnected.Connected;


}

void UViconRetimingClient::DataStream_GetSubjectCount(int32 & SubjectCount)
{

	 
	RetimingSubjectCount = MyRetimingClient.GetSubjectCount();
	if (RetimingSubjectCount.Result == ViconDataStreamSDK::CPP::Result::Success)
	{
		SubjectCount = RetimingSubjectCount.SubjectCount;
	}

}

void UViconRetimingClient::DataStream_GetSubjectName(int32 Count, TArray<FString>& SubjectNames)
{

	UE_LOG(LogTemp, Warning, TEXT("Subjects Count : %d"), Count);

	for (int32 SubjectIndex = 0; SubjectIndex < Count; ++SubjectIndex)
	{


		RetimingSubjectName = MyRetimingClient.GetSubjectName(SubjectIndex);

		SubjectNames.Empty();
		bool ErrorLog = true;
		FString   result = "GetSubjectName :";
		std::string SubjectNameStr = "";
		switch (RetimingSubjectName.Result)
		{
		case  ViconDataStreamSDK::CPP::Result::Success:
			ErrorLog = false;
			SubjectNameStr = RetimingSubjectName.SubjectName;
			RC_SubjectName = SubjectNameStr.c_str();

			SubjectNames.Add(RC_SubjectName);
			break;
		case ViconDataStreamSDK::CPP::Result::NotConnected:
			result += "NotConnected";
			break;
		case ViconDataStreamSDK::CPP::Result::NoFrame:
			result += "NoFrame";
			break;
		case ViconDataStreamSDK::CPP::Result::InvalidIndex:
			result += "InvalidSubjectName";
			break;

		}
		if (ErrorLog)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
			return;
		}

	}
}

void UViconRetimingClient::DataStream_SegmentCount(FString SubjectName, int32 &Count)
{
	
	RetimingSegmentCount = MyRetimingClient.GetSegmentCount(TCHAR_TO_UTF8(*SubjectName));
	if (RetimingSegmentCount.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{

		Count = RC_SegmentCount = RetimingSegmentCount.SegmentCount;

		UE_LOG(LogTemp, Warning, TEXT("Test :DataStream_SegmentCount : %d"), RC_SegmentCount);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Segment Count : %d"), RC_SegmentCount));
	}
}

void UViconRetimingClient::DataStream_GetSegmentName(FString SubjectName, int32 Count, TArray<FString>& SegmentNames)
{
	 
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);

	for (int32 SegmentIndex = 0; SegmentIndex < Count; ++SegmentIndex)
	{
		RetimingSegmentName = MyRetimingClient.GetSegmentName(_SubjectName, SegmentIndex);

		bool ErrorLog = true;
		FString result = "GetSegmentName = ";
		std::string _SegmentName;
		switch (RetimingSegmentName.Result)
		{
		case ViconDataStreamSDK::CPP::Result::Success:
			ErrorLog = false;
			_SegmentName = RetimingSegmentName.SegmentName;
			RC_SegmentName = _SegmentName.c_str();
			SegmentNames.Add(RC_SegmentName);
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
		}

		if (ErrorLog)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
		}


	}


}

void UViconRetimingClient::DataStream_GetSegmentLocalTranslation(FString SubjectName, FString SegmentName, FVector & NewLocation)
{
	MyRetimingClient.UpdateFrame(FrameOffset);
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	RetimingSegmentLocalTrasnslation = MyRetimingClient.GetSegmentLocalTranslation(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Local Translation :";

	FString LocXStr;
	FString LocYStr;
	FString LocZStr;

	switch (RetimingSegmentLocalTrasnslation.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewLocation.X = RetimingSegmentLocalTrasnslation.Translation[0] * RC_XYOffsets;
		NewLocation.Y = RetimingSegmentLocalTrasnslation.Translation[1];
		NewLocation.Z = RetimingSegmentLocalTrasnslation.Translation[2];

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

void UViconRetimingClient::DataStream_GetSegmentLocalRotationQuaternion(FString SubjectName, FString SegmentName, FVector & NewRotation)
{
	MyRetimingClient.UpdateFrame(FrameOffset);
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	RetimingSegmentLocalRotationQuaternion = MyRetimingClient.GetSegmentLocalRotationQuaternion(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Local Rotation_Quaternion :";
	float QuatW;
	float QuatX;
	float QuatY;
	float QuatZ;
	FString QuatWStr;
	FString QuatXStr;
	FString QuatYStr;
	FString QuatZStr;
	switch (RetimingSegmentLocalRotationQuaternion.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		QuatW = RetimingSegmentLocalRotationQuaternion.Rotation[0];
		QuatX = RetimingSegmentLocalRotationQuaternion.Rotation[1];
		QuatY = RetimingSegmentLocalRotationQuaternion.Rotation[2] + RC_PitchOffsets;
		QuatZ = RetimingSegmentLocalRotationQuaternion.Rotation[3];


		NewRotation = FQuat(QuatW, QuatX, QuatY, QuatZ).Euler();

		NewRotation.Y *= RC_XYOffsets;
		NewRotation.Z *= RC_XYOffsets;

		 
		 

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
