// Fill out your copyright notice in the Description page of Project Settings.


#include "Infos/TrivialKartPlayerState.h"

#include "GameInstances/TrivialKartGameInstance.h"

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
Instance->AddAchievementProgress(100, DistanceAchievementName, DistanceAchievementID);
		}
	}
}

float ATrivialKartPlayerState::GetFuel() const
{
	return Fuel;
}

float ATrivialKartPlayerState::GetDistance() const
{
	return Distance;
}

void ATrivialKartPlayerState::UpdateFuel()
{
	const float FuelPercentage = Fuel / 100.0f;
	OnFuelUpdated.ExecuteIfBound(FuelPercentage);
}

void ATrivialKartPlayerState::UpdateDistance()
{
	OnDistanceUpdated.ExecuteIfBound(Distance);
}
