// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GarageWidget.h"

#include "Actors/TrivialKartHUD.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"

void UGarageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BackButton->OnClicked.AddUniqueDynamic(this, &UGarageWidget::OnBackButtonClicked);
	CheckBoxes.Empty();
	CheckBoxes.Add(KartCheckBox);
	CheckBoxes.Add(LevelCheckBox);
	KartCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UGarageWidget::OnKartBoxStateChanged);
	LevelCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UGarageWidget::OnLevelBoxStateChange);
}

void UGarageWidget::NativeDestruct()
{
	BackButton->OnClicked.RemoveDynamic(this, &UGarageWidget::OnBackButtonClicked);
	Super::NativeDestruct();
}

void UGarageWidget::OnBackButtonClicked()
{
	if (TWeakObjectPtr PlayerController = GetOwningPlayer(); PlayerController.IsValid())
	{
		if (const TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(PlayerController->GetHUD()); 
			CurrentHUD.IsValid())
		{
			CurrentHUD->AddWidgetToScreen(EWidgetType::PlayBoard);
			CurrentHUD->RemoveWidgetFromScreen(EWidgetType::Garage);
		}
	}
}

void UGarageWidget::SetCheckBoxState(bool bIsChecked, UCheckBox* CheckBox, int ActiveWidgetIndex)
{
	if (CheckBox == nullptr)
		return;
	if (bIsChecked)
	{
		ContentSwitcher->SetActiveWidgetIndex(ActiveWidgetIndex);
		for (auto CurrentCheckBox : CheckBoxes)
		{
			if (CurrentCheckBox != CheckBox)
			{
				CurrentCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
			}
		}
	}
	else
	{
		CheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UGarageWidget::OnKartBoxStateChanged(bool bIsChecked)
{
	SetCheckBoxState(bIsChecked, KartCheckBox, 0);
}

void UGarageWidget::OnLevelBoxStateChange(bool bIsChecked)
{
	SetCheckBoxState(bIsChecked, LevelCheckBox, 1);
}
