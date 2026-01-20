#include "Widgets/PlayBoardWidget.h"

#include "Actors/TrivialKartHUD.h"
#include "Components/Button.h"


void UPlayBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GarageButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnGarageButtonClicked);
	PGSButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPGSButtonClicked);
	StoreButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStoreButtonClicked);
}

void UPlayBoardWidget::NativeDestruct()
{
	GarageButton->OnClicked.RemoveDynamic(this, &ThisClass::OnGarageButtonClicked);
	PGSButton->OnClicked.RemoveDynamic(this, &ThisClass::OnPGSButtonClicked);
	StoreButton->OnClicked.RemoveDynamic(this, &ThisClass::OnStoreButtonClicked);
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
