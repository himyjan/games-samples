// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectableItem.generated.h"

class UButtonWidgetStyle;
class UButton;
class SButton;
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API USelectableItem : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SelectionButton;
	
	UPROPERTY(EditAnywhere)
	FButtonStyle SelectionButtonStyle;
	
public:
	void NativePreConstruct() override;
};
