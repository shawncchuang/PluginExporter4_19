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

		
		/* DataStream Marker */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMakerGolbalTranslation(FString SubjectName, FString MakerName, FVector& NewLocation);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMarkerCount(FString SubjectName, int32 &MakerCount);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetLabelMarkerCount();

		/* DataStream Segment */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_SegmentCount(FString SubjectName);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSubjectRootSegmentName(FString SubjectName, FString& RootSegmentName);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalTranslation(FString SubjectName, FString SegementName, FVector& NewLocaroin);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalRotationEuler(FString SubjectName, FString SegmentName, FVector& NewRotation);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalRotationQuaternion(FString SubjectName, FString SegmentName, FVector& NewRotation);
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentGlobalRotationEulerXYZ(FString SubjectName, FString SegmentName, FVector& NewRotation);

private :
	UFUNCTION()
		void DebugMessage(FString Message);
	 
};
#define CPP PCPP