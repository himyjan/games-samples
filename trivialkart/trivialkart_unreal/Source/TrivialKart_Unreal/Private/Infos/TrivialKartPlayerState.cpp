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
	
	if (TWeakObjectPtr Instance = Cast<UTrivialKartGameInstance>(GetGameInstance()); 
			Instance.IsValid())
	{
		CoinPurchaseHandle = Instance->OnPurchaseReceived.AddUObject(this, &ATrivialKartPlayerState::OnPurchaseReceived);
	}
}

void ATrivialKartPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TWeakObjectPtr Instance = Cast<UTrivialKartGameInstance>(GetGameInstance()); 
			Instance.IsValid())
	{
		Instance->OnPurchaseReceived.Remove(CoinPurchaseHandle);
	}
	Super::EndPlay(EndPlayReason);
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

void ATrivialKartPlayerState::OnPurchaseReceived(const FString& PurchaseItemID, int Quantity)
{
	if (PurchaseItemID == CoinItemID)
	{
		CoinCount += Quantity;
		OnCoinUpdated.ExecuteIfBound(CoinCount);
	}
}
