// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SelectableItem.h"

#include "Components/Button.h"
#include "GameInstances/TrivialKartGameInstance.h"

void USelectableItem::NativePreConstruct()
{
	Super::NativePreConstruct();
	SelectionButton->SetStyle(SelectionButtonStyle);
}

void USelectableItem::NativeConstruct()
{
	Super::NativeConstruct();
	SelectionButton->OnClicked.AddUniqueDynamic(this, &USelectableItem::OnSelectionButtonClicked);
}

void USelectableItem::NativeDestruct()
{
	SelectionButton->OnClicked.RemoveDynamic(this, &USelectableItem::OnSelectionButtonClicked);
	Super::NativeDestruct();
}

void USelectableItem::OnSelectionButtonClicked()
{
	switch (SelectionOperationType)
	{
	case ESelectableOperationType::Purchase:
		{
			if (const TWeakObjectPtr Instance = Cast<UTrivialKartGameInstance>(GetGameInstance()); 
				Instance.IsValid())
			{
				Instance->StartPurchasing(PurchaseID, PurchaseQuantity);
			}
		}
		break;
	}
}
