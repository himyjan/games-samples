// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GarageWidget.generated.h"

class UWidgetSwitcher;
class UCheckBox;
class UButton;
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API UGarageWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> KartCheckBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> LevelCheckBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> ContentSwitcher;

protected:
	UPROPERTY()
	TArray<UCheckBox*> CheckBoxes;
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
private:
	UFUNCTION()
	void OnBackButtonClicked();
	void SetCheckBoxState(bool bIsChecked, UCheckBox* CheckBox, int ActiveWidgetIndex);
	UFUNCTION()
	void OnKartBoxStateChanged(bool bIsChecked);
	UFUNCTION()
	void OnLevelBoxStateChange(bool bIsChecked);
};
