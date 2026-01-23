// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectableItem.generated.h"


UENUM()
enum class ESelectableOperationType : uint8
{
	None,
	Purchase
};
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
	
	UPROPERTY(EditAnywhere)
	ESelectableOperationType SelectionOperationType;
	
	UPROPERTY(EditAnywhere)
	FString PurchaseID;
	
	UPROPERTY(EditAnywhere)
	int PurchaseQuantity;
	
public:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	UFUNCTION()
	void OnSelectionButtonClicked();
};
