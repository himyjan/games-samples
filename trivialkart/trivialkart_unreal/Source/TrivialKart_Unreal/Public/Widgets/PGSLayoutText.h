// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGSLayoutText.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API UPGSLayoutText : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PGSText;
	
public:
	void SetPGSText(const FString& Property, const FString& Value);
};
