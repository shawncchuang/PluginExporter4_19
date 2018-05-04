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
		/** Setting of Permission  for DataStream */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
		static void DataStream_Setting(bool EnableSegmentData, bool EnableMarkerData, bool EnableUnlabeledMarkerData,
																bool  EnableMarkerRayData, bool EnableDeviceData, bool EnableCentroidData,
																bool EnableGreyscaleData, bool EnableVideoData, bool EnableDebugData, bool UnrealEngineAxisStyle);

		/** Get the version of the Vicon DataStream SDK. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
		static void DataStream_GetSDKVersion(FString& SDKVersion);

		/** The function defaults to connecting on port 801. You can specify an alternate port number after a colon.
		This is for future compatibility : current products serve data on port 801 only. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
		static void DataStream_Connect(FString ServerName);

		/** Disconnect from the Vicon DataStream Server.*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_Disconnect();

		/** Discover whether client is connected to the Vicon DataStream Server. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_CheckConnected(bool& IsConnected);
	
		
		/** Vicon Data uses a right-handed coordinate system, with +X forward, +Y left, and +Z up. Other systems use different coordinate systems.
		Common usages are Z-up: SetAxisMapping( Forward, Left, Up) Y-up: SetAxisMapping( Forward, Up, Right )*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_AxisMapping(bool AxisMapping);

		/** Return the number of subjects in the DataStream. This information can be used in conjunction with GetSubjectName.*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSubjectCount(int32& SubjectCount);

		/** Return the name of a subject. This can be passed into segment and marker functions.*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSubjectName( int32 Count , TArray<FString>& SubjectNames);

		
		/** Return the name of a marker for a specified subject. This can be passed into GetMarkerGlobalTranslation. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMarkerGolbalTranslation(FString SubjectName, FString MakerName, FVector& NewLocation);
		/** Return the number of markers for a specified subject in the DataStream. This information can be used in conjunction with GetMarkerName. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMarkerCount(FString SubjectName, int32 &MarkerCount);
		/** Return the name of a marker for a specified subject. This can be passed into GetMarkerGlobalTranslation.*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetMarkerName(FString SubjectName, int32  Count, TArray<FString>& MarkerNames);
		/** Returns the number of all labeled markers in the data stream across all subjects.
		This may be used to determine marker index range for use with GetLabeledMarkerGlobalTranslation().*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetLabelMarkerCount();


		/** Return the number of segments for a specified subject in the DataStream.
		This information can be used in conjunction with GetSegmentName. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_SegmentCount(FString SubjectName, int32 & Count);
		/** Return the name of a subject segment specified by index. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentName(FString SubjectName, int32  Count, TArray<FString>& SegmentNames);
		/** Return the name of the root segment for a specified subject.
		This can be passed into segment functions.The root segment is the ancestor of all other segments in the subject. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSubjectRootSegmentName(FString SubjectName, FString& RootSegmentName);
		/** Return the translation of a subject segment in local coordinates relative to its parent segment. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalTranslation(FString SubjectName, FString SegementName, FVector& NewLocation);
		/** Return the translation of a subject segment in global coordinates.
		The translation is of the form ( x, y, z ) where x, y and z are in millimeters with respect to the global origin. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentGlobalTranslation(FString SubjectName, FString SegementName, FVector& NewLocation);
		/** Return the rotation of a subject segment in local Euler XYZ coordinates relative to its parent segment. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalRotationEulerXYZ(FString SubjectName, FString SegmentName, FVector& NewRotation);
		/** Return the rotation of a subject segment in global Euler XYZ coordinates. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentGlobalRotationEulerXYZ(FString SubjectName, FString SegmentName, FVector& NewRotation);
		/** Return the rotation of a subject segment in local quaternion coordinates relative to its parent segment.
		The quaternion is of the form (x, y, z, w) where w is the real component and x, y and z are the
		imaginary components. N.B. This is different from that used in many other applications, which segment (w, x, y, z).*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalRotationQuaternion(FString SubjectName, FString SegmentName, FVector& NewRotation);
		/**Return the rotation of a subject segment in global quaternion coordinates.
		The quaternion is of the form (x, y, z, w) where w is the real component and x, y and z are the
		imaginary components. N.B. This is different from that used in many other applications, which use (w, x, y, z).*/
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentGlobalRotationQuaternion(FString SubjectName, FString SegmentName, FVector& NewRotation);
		/** Return the rotation of a subject segment in local helical coordinates relative to its parent segment. */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentLocalRotationHelical(FString SubjectName, FString  SegmentName, FVector& NewRotation);
		/** Return the rotation of a subject segment in global quaternion coordinates.
		The quaternion is of the form (x, y, z, w) where w is the real component and x, y and z are the
		imaginary components. N.B. This is different from that used in many other applications, which use (w, x, y, z). */
		UFUNCTION(BlueprintCallable, Category = "Vicon")
			static void DataStream_GetSegmentGlobalRotationHelical(FString SubjectName, FString  SegmentName, FVector& NewRotation);
	


		
private :
 
	UFUNCTION()
		void DebugMessage(FString Message);


 
	 

};
#define CPP PCPP