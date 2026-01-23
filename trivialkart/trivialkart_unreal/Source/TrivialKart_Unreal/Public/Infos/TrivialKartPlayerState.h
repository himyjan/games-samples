// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TrivialKartPlayerState.generated.h"

DECLARE_DELEGATE_OneParam(FDistanceUpdated, const float);
DECLARE_DELEGATE_OneParam(FFuelUpdated, const float);

/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API ATrivialKartPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	float Fuel;
	
	float Distance;
	
	UPROPERTY(EditAnywhere)
	FString FuelAchievementName;
	
	UPROPERTY(EditAnywhere)
	FString FuelAchievementID;
	
	UPROPERTY(EditAnywhere)
	FString DistanceAchievementName;
	
	UPROPERTY(EditAnywhere)
	FString DistanceAchievementID;
	
	UPROPERTY(EditAnywhere)
	float DistanceAchievementThreshold;
	
public:
	void BeginPlay() override;
	
	void ConsumeFuel(const float FuelConsumption);
	void AddDistance(const float DistanceTravelled);
	
	float GetFuel() const;
	float GetDistance() const;
	
	FDistanceUpdated OnDistanceUpdated;
	FFuelUpdated OnFuelUpdated;
	
private:
	void UpdateFuel();
	void UpdateDistance();
};
