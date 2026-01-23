#include "Actors/TrivialKartHUD.h"

#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateHUD);

void ATrivialKartHUD::BeginPlay()
{
	Super::BeginPlay();
	AddWidgetToScreen(EWidgetType::PlayBoard);
}

void ATrivialKartHUD::AddWidgetToScreen(const EWidgetType WidgetType, int32 ZOrder)
{
	if (!WidgetTemplates.Contains(WidgetType))
	{
UE_LOG(LogTemplateHUD, Warning, TEXT("Widget template does not exist"));
		return;
	}
	const TWeakObjectPtr CurrentWidget = CreateWidget(GetOwningPlayerController(), 
		WidgetTemplates[WidgetType]);
	if (CurrentWidget.IsValid())
	{
		CurrentWidget->AddToViewport(ZOrder);
		ActiveWidgets.Emplace(WidgetType, CurrentWidget);
	}
}

void ATrivialKartHUD::RemoveWidgetFromScreen(const EWidgetType WidgetType)
{
	if (!ActiveWidgets.Contains(WidgetType))
	{
		UE_LOG(LogTemplateHUD, Warning, TEXT("Widget is not on Screen"));
		return;
	}
	ActiveWidgets[WidgetType]->RemoveFromParent();
	ActiveWidgets.Remove(WidgetType);
}

UUserWidget* ATrivialKartHUD::GetWidgetOfType(const EWidgetType WidgetType)
{
	if (!ActiveWidgets.Contains(WidgetType))
	{
UE_LOG(LogTemplateHUD, Warning, TEXT("Active widget does not exist"));
		return nullptr;
	}
	return ActiveWidgets[WidgetType].Get();
}
