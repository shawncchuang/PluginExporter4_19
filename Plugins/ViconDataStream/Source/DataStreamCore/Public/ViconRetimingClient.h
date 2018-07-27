// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define PCPP CPP
#undef CPP

#include <DataStreamRetimingClient.h>
#include <IDataStreamClientBase.h>

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Core/Public/Math/Quat.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViconRetimingClient.generated.h"

/**
 * 
 */
UCLASS()
class DATASTREAMCORE_API UViconRetimingClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
		/** Get the version of the Vicon DataStream SDK. */
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_GetSDKVersion(FString& SDKVersion);
	
		/** The function defaults to connecting on port 801. You can specify an alternate port number after a colon.
		This is for future compatibility : current products serve data on port 801 only. */
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_Connect(FString ServerName, float Offest);

		/** Discover whether client is connected to the Vicon DataStream Server. */
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_CheckConnected(bool& IsConnected);

		/** Return the number of subjects in the DataStream. This information can be used in conjunction with GetSubjectName.*/
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_GetSubjectCount(int32& SubjectCount);

		/** Return the name of a subject. This can be passed into segment and marker functions.*/
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_GetSubjectName(int32 Count, TArray<FString>& SubjectNames);

		/** Return the number of segments for a specified subject in the DataStream.
		This information can be used in conjunction with GetSegmentName. */
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_SegmentCount(FString SubjectName, int32& Count);

		/** Return the name of a subject segment specified by index. */
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_GetSegmentName(FString SubjectName, int32  Count, TArray<FString>& SegmentNames);

		/** Return the translation of a subject segment in local coordinates relative to its parent segment. */
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_GetSegmentLocalTranslation(FString SubjectName, FString SegementName, FVector& NewLocation);


		/** Return the rotation of a subject segment in local quaternion coordinates relative to its parent segment.
		The quaternion is of the form (x, y, z, w) where w is the real component and x, y and z are the
		imaginary components. N.B. This is different from that used in many other applications, which segment (w, x, y, z).*/
		UFUNCTION(BlueprintCallable, Category = "Vicon Retiming")
		static void DataStream_GetSegmentLocalRotationQuaternion(FString SubjectName, FString SegmentName, FVector& NewRotation);

};


#define CPP PCPP