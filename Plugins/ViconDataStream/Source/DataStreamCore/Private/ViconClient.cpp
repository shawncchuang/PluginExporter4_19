// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ViconClient.h"


ViconDataStreamSDK::CPP::Client MyClient;
ViconDataStreamSDK::CPP::Output_IsConnected ReIsConnected;


void UViconClient::GetSDKVersion()
{

	
	ViconDataStreamSDK::CPP::Output_GetVersion CurrentVersion = MyClient.GetVersion();
	float  _CurrentVersion = CurrentVersion.Major;

	//UE_LOG(LogTemp, Warning, TEXT("Test :ViconDataStramSDK Version: %f"), _CurrentVersion);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ViconDataStramSDK Version : %f"), _CurrentVersion));
}

void UViconClient::DataStreamConnect(FString ServerName, bool &IsConnected)
{
 
	MyClient.Connect(TCHAR_TO_UTF8(*ServerName));
	ReIsConnected = MyClient.IsConnected();
	IsConnected = ReIsConnected.Connected;
	
}

