// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OnlineSubsystemGooglePlayExtensionRuntimeSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Engine, DefaultConfig)
class ONLINESUBSYSTEMGOOGLEPLAYEXTENSIONEDITOR_API UOnlineSubsystemGooglePlayExtensionRuntimeSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, config, Category = PlayGameServices, Meta = (DisplayName = "Use Recall API"))
	bool bEnableRecall;
	
	UPROPERTY(EditAnywhere, config, Category = PlayGameServices, Meta = (DisplayName = "Default Icom for a Cloud Save File"))
	TSoftObjectPtr<UTexture2D> CloudSaveDefaultIcon;
	
	UPROPERTY(EditAnywhere, config, Category = PlayGameServices, Meta = (DisplayName = "Default Description for a Cloud Save File"))
	FString CloudSaveDefaultDescription;
	
	UPROPERTY(EditAnywhere, config, Category = PlayGameServices, Meta = (DisplayName = "Max Friends Search Limit"))
	int32 MaxFriendsSearchLimit;
	
	// UObject interface
	virtual void PostInitProperties() override;
	
};
