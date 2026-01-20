#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TrivialKartHUD.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateHUD, Log, All);

UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None,
	PlayBoard
};
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API ATrivialKartHUD : public AHUD
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetTemplates;
	
	UPROPERTY()
	TMap<EWidgetType, TWeakObjectPtr<UUserWidget>> ActiveWidgets;
	
public:
	virtual void BeginPlay() override;
	
	void AddWidgetToScreen(const EWidgetType WidgetType, int ZOrder = 0);
	
	UUserWidget* GetWidgetOfType(const EWidgetType WidgetType);
	
	
};
