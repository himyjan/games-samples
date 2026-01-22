// Fill out your copyright notice in the Description page of Project Settings.


#include "Infos/TrivialKartPlayerState.h"

#include "Actors/TrivialKartHUD.h"
#include "GameInstances/TrivialKartGameInstance.h"
#include "Widgets/PlayBoardWidget.h"

void ATrivialKartPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	Fuel = 100.0f;
	Distance = 0.0f;
	
	UpdateFuel();
	UpdateDistance();
}

void ATrivialKartPlayerState::ConsumeFuel(const float FuelConsumption)
{
	Fuel -= FuelConsumption;
	UpdateFuel();
	if (Fuel <= 0.0f)
	{
		if (TWeakObjectPtr Instance = Cast<UTrivialKartGameInstance>(GetGameInstance()); 
			Instance.IsValid())
		{
			Instance->AddAchievementProgress(100, FuelAchievementName, FuelAchievementID);
		}
	}
}

void ATrivialKartPlayerState::AddDistance(const float DistanceTravelled)
{
	Distance += DistanceTravelled;
	UpdateDistance();
	if (Distance >= DistanceAchievementThreshold)
	{
		if (TWeakObjectPtr Instance = Cast<UTrivialKartGameInstance>(GetGameInstance()); 
			Instance.IsValid())
		{
			Instance->AddAchievementProgress(100, DistanceAchievementID, DistanceAchievementName);
		}
	}
}

float ATrivialKartPlayerState::GetFuel() const
{
	return Fuel;
}

void ATrivialKartPlayerState::UpdateFuel()
{
	if (TWeakObjectPtr CurrentController = GetPlayerController(); CurrentController.IsValid())
	{
		if (TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(
			GetPlayerController()->GetHUD()); CurrentHUD.IsValid())
		{
			if (TWeakObjectPtr Widget = Cast<UPlayBoardWidget>(
				CurrentHUD->GetWidgetOfType(EWidgetType::PlayBoard)); Widget.IsValid())
			{
				const float FuelPercentage = Fuel / 100.0f;
				Widget->UpdateFuelBar(FuelPercentage);
			}
		}
	}
}

void ATrivialKartPlayerState::UpdateDistance()
{
	if (TWeakObjectPtr CurrentController = GetPlayerController(); CurrentController.IsValid())
	{
		if (TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(
			GetPlayerController()->GetHUD()); CurrentHUD.IsValid())
		{
			if (TWeakObjectPtr Widget = Cast<UPlayBoardWidget>(
				CurrentHUD->GetWidgetOfType(EWidgetType::PlayBoard)); Widget.IsValid())
			{
				Widget->UpdateDistanceText(Distance);
			}
		}
	}
}
