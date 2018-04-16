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
		static void GetSDKVersion();

		UFUNCTION(BlueprintCallable, Category = "Vicon")
		static void DataStreamConnect(FString ServerName, bool& IsConnected);
	
	
	
};
#define CPP PCPP