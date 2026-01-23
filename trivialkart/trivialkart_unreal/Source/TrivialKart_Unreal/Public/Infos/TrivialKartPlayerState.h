// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TrivialKartPlayerState.generated.h"

DECLARE_DELEGATE_OneParam(FDistanceUpdated, const float);
DECLARE_DELEGATE_OneParam(FFuelUpdated, const float);
DECLARE_DELEGATE_OneParam(FCoinUpdated, const int);

/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API ATrivialKartPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	float Fuel;
	
	float Distance;
	
	int CoinCount;
	
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
	
	UPROPERTY(EditAnywhere)
	FString CoinItemID;
	
	FDelegateHandle CoinPurchaseHandle;
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void ConsumeFuel(const float FuelConsumption);
	void AddDistance(const float DistanceTravelled);
	
	float GetFuel() const;
	float GetDistance() const;
	
	FDistanceUpdated OnDistanceUpdated;
	FFuelUpdated OnFuelUpdated;
	FCoinUpdated OnCoinUpdated;
	
private:
	void UpdateFuel();
	void UpdateDistance();
	
	UFUNCTION()
	void OnPurchaseReceived(const FString& PurchaseItemID, int Quantity);
};
