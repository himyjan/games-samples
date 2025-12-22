#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "TrivialKartGameInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateGameInstance, Log, All);
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API UTrivialKartGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance|Authentication")
	void InitiateAutoLogin();
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance|Authentication")
	bool GetLoginStatus() const;
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance|Authentication")
	FString GetPlayerName() const;
	
private:
	void OnLoginCompleted(int I, bool bArg, const FUniqueNetId& UniqueNetId, const FString& String) const;
	
};
