#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayBoardWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API UPlayBoardWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> GarageButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> PGSButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StoreButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DistanceText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CoinText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> FuelBar;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UFUNCTION()
	void OnGarageButtonClicked();
	UFUNCTION()
	void OnPGSButtonClicked();
	UFUNCTION()
	void OnStoreButtonClicked();
	
public:
	void UpdateFuelBar(const float FuelBarPercentage) const;
	void UpdateDistanceText(const float Distance) const;
};
