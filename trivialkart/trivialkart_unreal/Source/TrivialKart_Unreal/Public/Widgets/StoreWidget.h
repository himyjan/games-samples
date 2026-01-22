// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StoreWidget.generated.h"

class UWidgetSwitcher;
class UCheckBox;
class UButton;
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API UStoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BackButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> FuelCheckBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> CoinCheckBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> CarCheckBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheckBox> SubscriptionCheckBox;
	
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
	void OnFuelBoxStateChanged(bool bIsChecked);
	UFUNCTION()
	void OnCoinBoxStateChange(bool bIsChecked);
	UFUNCTION()
	void OnCarBoxStateChange(bool bIsChecked);
	UFUNCTION()
	void OnSubscriptionBoxStateChange(bool bIsChecked);
};
