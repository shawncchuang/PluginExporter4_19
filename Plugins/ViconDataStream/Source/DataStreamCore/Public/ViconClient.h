// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define PCPP CPP
#undef CPP


#include <DataStreamRetimingClient.h>
#include <DataStreamClient.h>
#include <IDataStreamClientBase.h>

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViconClient.generated.h"

/**
 * 
 */
UCLASS()
class DATASTREAMCORE_API UViconClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

		UFUNCTION(BlueprintCallable, Category = "Vicon")
		static void DataStream_GetSDKVersion();

		UFUNCTION(BlueprintCallable, Category = "Vicon")
		static void DataStream_Connect(FString ServerName, bool& IsConnected);

		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_CheckConnected(bool& IsConnected);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSubjectCount(int32& SubjectCount);

		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSubjectName(FString& SubjectName);

		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_SegmentCount(FString SubjectName);

		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMakerGolbalTranslation(FString SubjectName, FString MakerName, FVector& NewLocation);

		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMarkerCount(FString SubjectName, int32 &MakerCount);

		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetLabelMarkerCount();

	
};
#define CPP PCPP