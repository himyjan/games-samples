// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SelectableItem.h"

#include "Components/Button.h"

void USelectableItem::NativePreConstruct()
{
	Super::NativePreConstruct();
	SelectionButton->SetStyle(SelectionButtonStyle);
}
