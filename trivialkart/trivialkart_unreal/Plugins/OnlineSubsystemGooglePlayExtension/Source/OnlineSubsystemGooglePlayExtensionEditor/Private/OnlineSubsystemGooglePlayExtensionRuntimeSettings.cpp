// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSubsystemGooglePlayExtensionRuntimeSettings.h"

void UOnlineSubsystemGooglePlayExtensionRuntimeSettings::PostInitProperties()
{
	Super::PostInitProperties();
	TryUpdateDefaultConfigFile(TEXT(""), false);
}
