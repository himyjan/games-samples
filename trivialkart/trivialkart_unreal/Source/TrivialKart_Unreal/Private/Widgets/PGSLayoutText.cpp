// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PGSLayoutText.h"

#include "Components/TextBlock.h"

void UPGSLayoutText::SetPGSText(const FString& Property, const FString& Value)
{
	PGSText->SetText(FText::FromString(Property + " : " + Value));
}
