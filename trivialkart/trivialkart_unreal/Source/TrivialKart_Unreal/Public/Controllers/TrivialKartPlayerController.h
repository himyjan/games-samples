// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TrivialKartPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API ATrivialKartPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
public:
	virtual void SetupInputComponent() override;
};
