// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ViconClient.h"
 


ViconDataStreamSDK::CPP::Client MyClient;
ViconDataStreamSDK::CPP::Output_GetVersion CurrentVersion;
ViconDataStreamSDK::CPP::Output_IsConnected ReIsConnected;
ViconDataStreamSDK::CPP::Output_SetAxisMapping ReSetAxisMapping;
ViconDataStreamSDK::CPP::Output_GetAxisMapping ReGetAxisMapping;
 
ViconDataStreamSDK::CPP::Output_GetSubjectName ReSubjectName;
ViconDataStreamSDK::CPP::Output_GetSubjectCount ReSubjectCount;
ViconDataStreamSDK::CPP::Output_GetMarkerCount ReMarkerCount;
ViconDataStreamSDK::CPP::Output_GetMarkerName ReMarkerName;
ViconDataStreamSDK::CPP::Output_GetMarkerGlobalTranslation ReMarkerTranslation;
ViconDataStreamSDK::CPP::Output_GetSegmentCount ReSegmentCount;
ViconDataStreamSDK::CPP::Output_GetLabeledMarkerCount ReLabeledMarkerCount;
ViconDataStreamSDK::CPP::Output_GetSegmentName ReSegmentName;
ViconDataStreamSDK::CPP::Output_GetSubjectRootSegmentName ReRootSegmentName;

ViconDataStreamSDK::CPP::Output_GetSegmentLocalTranslation ReSegmentLocalTrasnslation;
ViconDataStreamSDK::CPP::Output_GetSegmentGlobalTranslation ReSegmentGlobalTrasnslation;

ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationEulerXYZ ReSegmentLocalRotationEuler;
ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationQuaternion ReSegmentLocalRotationQuaternion;

ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationEulerXYZ ReSegmentGlobalRotationEuler;
ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationQuaternion ReSegmentGlobalRotationQuaternion;

ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationHelical ReSegmentGlobalRotationHelical;
ViconDataStreamSDK::CPP::Output_GetSegmentLocalRotationHelical ReSegmentLocalRotationHelical;


bool _EnableSegmentData;
bool _EnableMarkerData;
bool _EnableUnlabeledMarkerData;
bool _EnableMarkerRayData;
bool _EnableDeviceData;
bool _EnableCentroidData;
bool _EnableGreyscaleData;
bool _EnableVideoData;
bool _EnableDebugData;
bool _IsConnected;
bool _LevelAxis;

FString ServerAddress;
int32  SegmentCount = 0;
int32  SubjectCount = 0;
int32 MakerCount = 0;
FString SubjectName;
FString MarkerName;
FString SegmentName;
int32  OnCentimeters = 10;
//Default Axis   X-Right , Y- Forward and Z-UP , UE4 Left-Hand of axis and Z-UP;
int32  PitchOffsets = 180;
int32  XYOffsets = -1;

void UViconClient::DataStream_Setting(bool EnableSegmentData, bool EnableMarkerData, bool EnableUnlabeledMarkerData,
	bool  EnableMarkerRayData, bool EnableDeviceData, bool EnableCentroidData,
	bool EnableGreyscaleData, bool EnableVideoData, bool EnableDebugData)
{

	_EnableSegmentData = EnableSegmentData;
	_EnableMarkerData = EnableMarkerData;
	_EnableUnlabeledMarkerData = EnableUnlabeledMarkerData;
	_EnableMarkerRayData = EnableMarkerRayData;
	_EnableDeviceData = EnableDeviceData;
	_EnableCentroidData = EnableCentroidData;
	_EnableGreyscaleData = EnableGreyscaleData;
	_EnableVideoData = EnableVideoData;
	_EnableDebugData = EnableDebugData;


}

void UViconClient::DataStream_GetSDKVersion(bool ShowLogOnSreen)
{

	
	CurrentVersion = MyClient.GetVersion();
	int32  _CurrentVersionMajor = CurrentVersion.Major;
	int32 _CurrentVersionPoint = CurrentVersion.Point;
	FString  _CurrentVersion = FString::FromInt(_CurrentVersionMajor) + "." + FString::FromInt(_CurrentVersionPoint);
	//UE_LOG(LogTemp, Warning, TEXT("Test :ViconDataStramSDK Version: %f"), _CurrentVersion);
	if(ShowLogOnSreen)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ViconDataStramSDK Version : %s"), *_CurrentVersion));
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

void UViconClient::DataStream_SetAxisMapping(TMap<FString, bool> AxisMapping)
{
	 
 
		// Return  LeftHandedAxis ; DataStream SDK did not support the axis with X-Forward, Y-Right, Z-Up  in Unreal Engine
		ReSetAxisMapping  = MyClient.SetAxisMapping(ViconDataStreamSDK::CPP::Direction::Forward,
			ViconDataStreamSDK::CPP::Direction::Right,
			ViconDataStreamSDK::CPP::Direction::Up);

		if (ReSetAxisMapping.Result == ViconDataStreamSDK::CPP::Result::Success) {
			UE_LOG(LogTemp, Warning, TEXT("Set Axis Mapping Success"));
		}
		if (ReSetAxisMapping.Result == ViconDataStreamSDK::CPP::Result::CoLinearAxes) {
			UE_LOG(LogTemp, Warning, TEXT("Set Axis Mapping CoLinearAxes"));
		}
		if (ReSetAxisMapping.Result == ViconDataStreamSDK::CPP::Result::LeftHandedAxes) {
			UE_LOG(LogTemp, Warning, TEXT("Set Axis Mapping LeftHandedAxes"));
		}
 
 
	ReGetAxisMapping = MyClient.GetAxisMapping();
	int32 XAxis = ReGetAxisMapping.XAxis;
	int32 YAxis = ReGetAxisMapping.YAxis;
	int32 ZAxis = ReGetAxisMapping.ZAxis;
	UE_LOG(LogTemp, Warning, TEXT("Get Axis Mapping : %d, %d, %d"), XAxis,YAxis, ZAxis);
	 
}


void UViconClient::DataStream_GetSubjectCount(int32 &SubjectCount)
{
	MyClient.GetFrame();
	ReSubjectCount = MyClient.GetSubjectCount();
	if (ReSubjectCount.Result == ViconDataStreamSDK::CPP::Result::Success)
	{
		SubjectCount = ReSubjectCount.SubjectCount;
	}

}

void UViconClient::DataStream_GetSubjectName( int32 Count, TArray<FString> &SubjectNames)
{
	UE_LOG(LogTemp, Warning, TEXT("Subjects Count : %d"), Count);
	 
	for (int32 SubjectIndex = 0 ; SubjectIndex<Count; ++SubjectIndex)
	{
		 
		MyClient.GetFrame();
		ReSubjectName = MyClient.GetSubjectName(SubjectIndex);

		bool ErrorLog = true;
		FString   result = "GetSubjectName :";
		std::string SubjectNameStr = "";
		switch (ReSubjectName.Result)
		{
		case  ViconDataStreamSDK::CPP::Result::Success:
			ErrorLog = false;
			SubjectNameStr = ReSubjectName.SubjectName;
			SubjectName = SubjectNameStr.c_str();
			  
			SubjectNames.Add(SubjectName);
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

void UViconClient::DataStream_GetMarkerCount(FString SubjectName, int32 &MarkerCount)
{

	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	MyClient.GetFrame();
	ReMarkerCount = MyClient.GetMarkerCount(_SubjectName);

	bool ErrorLog = true;
	FString   result = "GetMarkerCount :";

	switch (ReMarkerCount.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;
		MarkerCount = ReMarkerCount.MarkerCount;
		break;
	case ViconDataStreamSDK::CPP::Result::NoFrame:
		result += "NoFrame";
		break;
	case  ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	case  ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
		result += "InvalidSubjectName";
		break;

	}

	if (ErrorLog)
	{
		MakerCount = -1;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
	}

 
}

void UViconClient::DataStream_GetMarkerName(FString SubjectName, int32  Count, TArray<FString> &MarkerNames)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Marker Count :  %d"), Count);
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	 
	for (int32  MarkIndex  = 0 ; MarkIndex < Count ; ++MarkIndex)
	{
		MyClient.GetFrame();
		ReMarkerName = MyClient.GetMarkerName(_SubjectName, MarkIndex);

		bool ErrorLog = true;
		FString   result = "GetMarkerName :";
		std::string _MarkerName = "";
		switch (ReMarkerName.Result)
		{
		case ViconDataStreamSDK::CPP::Result::Success:
			ErrorLog = false;
			_MarkerName = ReMarkerName.MarkerName;
			MarkerName = _MarkerName.c_str();

			MarkerNames.Add(MarkerName);
			break;
		case ViconDataStreamSDK::CPP::Result::NoFrame:
			result += "NoFrame";
			break;
		case  ViconDataStreamSDK::CPP::Result::NotConnected:
			result += "NotConnected";
			break;
		case  ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
			result += "InvalidSubjectName";
			break;
		case  ViconDataStreamSDK::CPP::Result::InvalidIndex:
			result += "InvalidIndex";
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


void UViconClient::DataStream_GetMarkerGolbalTranslation(FString SubjectName, FString MarkerName, FVector &NewLocation)
{
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _MarkerName = TCHAR_TO_UTF8(*MarkerName);
	MyClient.GetFrame();
	ReMarkerTranslation = MyClient.GetMarkerGlobalTranslation(_SubjectName, _MarkerName);

	bool ErrorLog = true;
	FString   result = "GetMakerGolbalTranslation :";

	switch (ReMarkerTranslation.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewLocation.X = ReMarkerTranslation.Translation[0] * XYOffsets;
		NewLocation.Y = ReMarkerTranslation.Translation[1];
		NewLocation.Z = ReMarkerTranslation.Translation[2];
		 /*
		for (double &_value : ReMarkerTranslation.Translation)
		{
			result += "," + FString::SanitizeFloat(_value);
		}
		*/

		break;
	case ViconDataStreamSDK::CPP::Result::NoFrame:
		result += "NoFrame";
		break;
	case  ViconDataStreamSDK::CPP::Result::NotConnected:
		result += "NotConnected";
		break;
	case  ViconDataStreamSDK::CPP::Result::InvalidSubjectName:
		result += "InvalidSubjectName";
		break;
	case  ViconDataStreamSDK::CPP::Result::InvalidMarkerName:
		result += "InvalidMarkerName";
		break;
 
	}

	if (ErrorLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *result));
	}

	 
	 
}

void UViconClient::DataStream_GetLabelMarkerCount()
{

	ReLabeledMarkerCount = MyClient.GetLabeledMarkerCount();
	if (ReLabeledMarkerCount.Result == ViconDataStreamSDK::CPP::Result::Success)
	{
		int32 LabeledMarkerCount = ReLabeledMarkerCount.MarkerCount;
		FString result = "Labeled Marker Count : "+ LabeledMarkerCount;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
	
	}


}


void UViconClient::DataStream_SegmentCount(FString SubjectName, int32 &Count)
{

	ReSegmentCount = MyClient.GetSegmentCount(TCHAR_TO_UTF8(*SubjectName));
	if (ReSegmentCount.Result == ViconDataStreamSDK::CPP::Result::Enum::Success)
	{
	 
		Count = SegmentCount = ReSegmentCount.SegmentCount;
		
		UE_LOG(LogTemp, Warning, TEXT("Test :DataStream_SegmentCount : %d"), SegmentCount);
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Segment Count : %d"),SegmentCount));
	}

}

void UViconClient::DataStream_GetSegmentName(FString SubjectName, int32 Count, TArray<FString> &SegmentNames) {

	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);

	for (int32 SegmentIndex = 0; SegmentIndex < Count ; ++SegmentIndex)
	{
		ReSegmentName = MyClient.GetSegmentName(_SubjectName,SegmentIndex);

		bool ErrorLog = true;
		FString result = "GetSegmentName = ";
		std::string _SegmentName;
		switch (ReSegmentName.Result)
		{
		case ViconDataStreamSDK::CPP::Result::Success:
			ErrorLog = false;
			_SegmentName = ReSegmentName.SegmentName;
			SegmentName = _SegmentName.c_str();
			SegmentNames.Add(SegmentName);
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

		NewLocation.X = ReSegmentLocalTrasnslation.Translation[0] * XYOffsets;
		NewLocation.Y= ReSegmentLocalTrasnslation.Translation[1];
		NewLocation.Z = ReSegmentLocalTrasnslation.Translation[2];

		/*
		for (double &value : ReSegmentLocalTrasnslation.Translation)
		{
			result += ", "+FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
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

void UViconClient::DataStream_GetSegmentGlobalTranslation(FString SubjectName, FString SegmentName, FVector &NewLocation)
{
	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);
	ReSegmentGlobalTrasnslation = MyClient.GetSegmentGlobalTranslation(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Local Translation :";
	switch (ReSegmentGlobalTrasnslation.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewLocation.X = ReSegmentGlobalTrasnslation.Translation[0] * XYOffsets;
		NewLocation.Y = ReSegmentGlobalTrasnslation.Translation[1];
		NewLocation.Z = ReSegmentGlobalTrasnslation.Translation[2];

		/*
		for (double &value : ReSegmentLocalTrasnslation.Translation)
		{
		result += ", "+FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
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


void UViconClient::DataStream_GetSegmentLocalRotationEulerXYZ(FString SubjectName, FString SegmentName, FVector &NewRotation)
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

		NewRotation.X = ReSegmentLocalRotationEuler.Rotation[0] * OnCentimeters;
		NewRotation.Y = ReSegmentLocalRotationEuler.Rotation[1] * OnCentimeters + PitchOffsets;
		NewRotation.Z = ReSegmentLocalRotationEuler.Rotation[2] * OnCentimeters;

		/*
		for (double &value : ReSegmentLocalRotationEuler.Rotation)
		{
		result += ", "+FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
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

		NewRotation.X = ReSegmentLocalRotationQuaternion.Rotation[0] * OnCentimeters;
		NewRotation.Y = ReSegmentLocalRotationQuaternion.Rotation[1] * OnCentimeters + PitchOffsets;
		NewRotation.Z = ReSegmentLocalRotationQuaternion.Rotation[2] * OnCentimeters;

		/*
		for (double &value : ReSegmentLocalRotationQuaternion.Rotation)
		{
			result += ", " + FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
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

		NewRotation.X = ReSegmentGlobalRotationEuler.Rotation[0] * OnCentimeters;
		NewRotation.Y = ReSegmentGlobalRotationEuler.Rotation[1] * OnCentimeters + PitchOffsets;
		NewRotation.Z = ReSegmentGlobalRotationEuler.Rotation[2] * OnCentimeters;

		/*
		for (double &value : ReSegmentGlobalRotationEuler.Rotation)
		{
			result += ", " + FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
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

void UViconClient::DataStream_GetSegmentGlobalRotationQuaternion(FString SubjectName, FString SegmentName , FVector  &NewRotation)
{

	MyClient.GetFrame();
	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);

	ReSegmentGlobalRotationQuaternion = MyClient.GetSegmentGlobalRotationQuaternion(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Global Rotation_Quaternion :";

	switch (ReSegmentGlobalRotationQuaternion.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;

		NewRotation.X = ReSegmentGlobalRotationQuaternion.Rotation[0] * OnCentimeters;
		NewRotation.Y = ReSegmentGlobalRotationQuaternion.Rotation[1] * OnCentimeters + PitchOffsets;
		NewRotation.Z = ReSegmentGlobalRotationQuaternion.Rotation[2] * OnCentimeters;

		/*
		for (double &value : ReSegmentGlobalRotationQuaternion.Rotation)
		{
		result += ", " + FString::SanitizeFloat(value);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *result);
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

void UViconClient::DataStream_GetSegmentGlobalRotationHelical(FString SubjectName, FString SegmentName, FVector &NewRotation)
{
	MyClient.GetFrame();

	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);

	ReSegmentGlobalRotationHelical = MyClient.GetSegmentGlobalRotationHelical(_SubjectName, _SegmentName);

	bool ErrorLog = true;  
	FString   result = "Segment Global Rotation_Helical :";

	switch (ReSegmentGlobalRotationHelical.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;
		NewRotation.X = ReSegmentGlobalRotationHelical.Rotation[0] * OnCentimeters;
		NewRotation.Y = ReSegmentGlobalRotationHelical.Rotation[1] * OnCentimeters + PitchOffsets;
		NewRotation.Z = ReSegmentGlobalRotationHelical.Rotation[2] * OnCentimeters;

 
		for (double &value : ReSegmentGlobalRotationHelical.Rotation)
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

void UViconClient::DataStream_GetSegmentLocalRotationHelical(FString SubjectName, FString SegmentName, FVector &NewRotation)
{
	MyClient.GetFrame();

	std::string _SubjectName = TCHAR_TO_UTF8(*SubjectName);
	std::string _SegmentName = TCHAR_TO_UTF8(*SegmentName);

	ReSegmentLocalRotationHelical = MyClient.GetSegmentLocalRotationHelical(_SubjectName, _SegmentName);

	bool ErrorLog = true;
	FString   result = "Segment Global Rotation_Helical :";

	switch (ReSegmentLocalRotationHelical.Result)
	{
	case ViconDataStreamSDK::CPP::Result::Success:
		ErrorLog = false;
		NewRotation.X = ReSegmentLocalRotationHelical.Rotation[0] * OnCentimeters;
		NewRotation.Y = ReSegmentLocalRotationHelical.Rotation[1] * OnCentimeters + PitchOffsets;
		NewRotation.Z = ReSegmentLocalRotationHelical.Rotation[2] * OnCentimeters;


		for (double &value : ReSegmentLocalRotationHelical.Rotation)
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



 
 