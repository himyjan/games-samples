#include "Widgets/PlayBoardWidget.h"

#include "Actors/TrivialKartHUD.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Infos/TrivialKartPlayerState.h"


void UPlayBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GarageButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnGarageButtonClicked);
	PGSButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPGSButtonClicked);
	StoreButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStoreButtonClicked);
	
	if (TWeakObjectPtr PlayerState = Cast<ATrivialKartPlayerState>(GetOwningPlayerState()); PlayerState.IsValid())
	{
		PlayerState->OnFuelUpdated.BindUObject(this, &UPlayBoardWidget::UpdateFuelBar);
		PlayerState->OnDistanceUpdated.BindUObject(this, &UPlayBoardWidget::UpdateDistanceText);
		PlayerState->OnCoinUpdated.BindUObject(this, &UPlayBoardWidget::UpdateCoinText);
		const float FuelPercentage = PlayerState->GetFuel() / 100.0f;
		UpdateFuelBar(FuelPercentage);
		UpdateDistanceText(PlayerState->GetDistance());
	}
}

void UPlayBoardWidget::NativeDestruct()
{
	GarageButton->OnClicked.RemoveDynamic(this, &ThisClass::OnGarageButtonClicked);
	PGSButton->OnClicked.RemoveDynamic(this, &ThisClass::OnPGSButtonClicked);
	StoreButton->OnClicked.RemoveDynamic(this, &ThisClass::OnStoreButtonClicked);
	
	if (TWeakObjectPtr PlayerState = Cast<ATrivialKartPlayerState>(GetOwningPlayerState()); PlayerState.IsValid())
	{
		PlayerState->OnFuelUpdated.Unbind();
		PlayerState->OnDistanceUpdated.Unbind();
		PlayerState->OnCoinUpdated.Unbind();
	}
	Super::NativeDestruct();
}

void UPlayBoardWidget::OnGarageButtonClicked()
{
	if (TWeakObjectPtr PlayerController = GetOwningPlayer(); PlayerController.IsValid())
	{
		if (const TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(PlayerController->GetHUD()); 
			CurrentHUD.IsValid())
		{
			CurrentHUD->AddWidgetToScreen(EWidgetType::Garage);
			CurrentHUD->RemoveWidgetFromScreen(EWidgetType::PlayBoard);
		}
	}
}

void UPlayBoardWidget::OnPGSButtonClicked()
{
	if (TWeakObjectPtr PlayerController = GetOwningPlayer(); PlayerController.IsValid())
	{
		if (const TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(PlayerController->GetHUD()); 
			CurrentHUD.IsValid())
		{
			CurrentHUD->AddWidgetToScreen(EWidgetType::PGS);
			CurrentHUD->RemoveWidgetFromScreen(EWidgetType::PlayBoard);
		}
	}
}

void UPlayBoardWidget::OnStoreButtonClicked()
{
	if (TWeakObjectPtr PlayerController = GetOwningPlayer(); PlayerController.IsValid())
	{
		if (const TWeakObjectPtr CurrentHUD = Cast<ATrivialKartHUD>(PlayerController->GetHUD()); 
			CurrentHUD.IsValid())
		{
			CurrentHUD->AddWidgetToScreen(EWidgetType::Store);
			CurrentHUD->RemoveWidgetFromScreen(EWidgetType::PlayBoard);
		}
	}
}

void UPlayBoardWidget::UpdateFuelBar(const float FuelBarPercentage) const
{
	FuelBar->SetPercent(FuelBarPercentage);
}

void UPlayBoardWidget::UpdateDistanceText(const float Distance) const
{
	DistanceText->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(Distance))));
}

void UPlayBoardWidget::UpdateCoinText(const int Quantity) const
{
	CoinText->SetText(FText::FromString(FString::FromInt(Quantity)));
}
