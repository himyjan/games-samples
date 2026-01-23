// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PGSWidget.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Actors/TrivialKartHUD.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameInstances/TrivialKartGameInstance.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Widgets/PGSLayoutText.h"

void UPGSWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackButton->OnClicked.AddUniqueDynamic(this, &UPGSWidget::OnBackButtonClicked);
	CheckBoxes.Empty();
	CheckBoxes.Add(AuthenticationCheckBox);
	CheckBoxes.Add(AchievementsCheckBox);
	AuthenticationCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UPGSWidget::OnAuthCheckBoxStateChanged);
	AchievementsCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UPGSWidget::OnAchievementBoxStateChange);
	if (TWeakObjectPtr Instance = Cast<UTrivialKartGameInstance>(GetGameInstance()); 
		Instance.IsValid())
	{
		FString AuthStatus = Instance->GetLoginStatus() ? FString("Logged In") : FString("Logged Out");
		if (auto PGSAuthStatusText = Cast<UPGSLayoutText>(
							CreateWidget(this, PGSTextTemplate)))
		{
			PGSAuthStatusText->SetPGSText("Auth Status", AuthStatus);
			AuthBox->AddChild(PGSAuthStatusText);
		}
		if (auto PGSUsernameText = Cast<UPGSLayoutText>(
							CreateWidget(this, PGSTextTemplate)))
		{
			PGSUsernameText->SetPGSText("Username", Instance->GetPlayerName());
			AuthBox->AddChild(PGSUsernameText);
		}
	}
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()); 
		IdentityInterface.IsValid())
	{
		if (const IOnlineAchievementsPtr AchievementsInterface = 
			Online::GetAchievementsInterface(GetWorld()); AchievementsInterface.IsValid())
		{
			TArray<FOnlineAchievement> Achievements;
			if (AchievementsInterface->GetCachedAchievements(
				*IdentityInterface->GetUniquePlayerId(0), Achievements) 
				== EOnlineCachedResult::Success)
			{
				for (auto Achievement : Achievements)
				{
					if (auto PGSText = Cast<UPGSLayoutText>(
							CreateWidget(this, PGSTextTemplate)))
					{
						PGSText->SetPGSText(Achievement.Id, FString::Printf(TEXT("%.1f"), Achievement.Progress));
						AchievementsBox->AddChild(PGSText);
					}
				}
			}
		}
	}
}

void UPGSWidget::NativeDestruct()
{
	BackButton->OnClicked.RemoveDynamic(this, &UPGSWidget::OnBackButtonClicked);
	AuthenticationCheckBox->OnCheckStateChanged.RemoveDynamic(this, &UPGSWidget::OnAuthCheckBoxStateChanged);
	AchievementsCheckBox->OnCheckStateChanged.RemoveDynamic(this, &UPGSWidget::OnAchievementBoxStateChange);
	Super::NativeDestruct();
}

void UPGSWidget::OnBackButtonClicked()
{
	if (TWeakObjectPtr PlayerController = GetOwningPlayer(); PlayerController.IsValid())
	{
		if (const TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(PlayerController->GetHUD()); 
			CurrentHUD.IsValid())
		{
			CurrentHUD->AddWidgetToScreen(EWidgetType::PlayBoard);
			CurrentHUD->RemoveWidgetFromScreen(EWidgetType::PGS);
		}
	}
}

void UPGSWidget::SetCheckBoxState(bool bIsChecked, UCheckBox* CheckBox, int ActiveWidgetIndex)
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

void UPGSWidget::OnAuthCheckBoxStateChanged(bool bIsChecked)
{
	SetCheckBoxState(bIsChecked, AuthenticationCheckBox, 0);
}

void UPGSWidget::OnAchievementBoxStateChange(bool bIsChecked)
{
	SetCheckBoxState(bIsChecked, AchievementsCheckBox, 1);
}
