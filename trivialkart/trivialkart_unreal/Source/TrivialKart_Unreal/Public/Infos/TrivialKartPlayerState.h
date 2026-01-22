// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TrivialKartPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API ATrivialKartPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	float Fuel;
	
	float Distance;
	
public:
	void BeginPlay() override;
	
	void ConsumeFuel(const float FuelConsumption);
	void AddDistance(const float DistanceTravelled);
	
	float GetFuel() const;
	
private:
	void UpdateFuel();
	void UpdateDistance();
};
