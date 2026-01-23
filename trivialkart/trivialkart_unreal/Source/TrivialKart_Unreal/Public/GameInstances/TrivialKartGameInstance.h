#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "TrivialKartGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPurchase, const FUniqueOfferId&, const int);
class FPurchaseReceipt;
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateGameInstance, Log, All);
/**
 * 
 */
UCLASS()
class TRIVIALKART_UNREAL_API UTrivialKartGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()
	
public:
	FOnPurchase OnPurchaseReceived;
	
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance|Authentication")
	void InitiateAutoLogin();
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance|Authentication")
	bool GetLoginStatus() const;
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance|Authentication")
	FString GetPlayerName() const;
	
	void AddAchievementProgress(const float Progress, const FString& AchievementName, const FString& AchievementID);
	void StartPurchasing(const FString& OfferID, const int32 Quantity);
	
protected:
	//A map for ID and if they are Consumable Purchased
	UPROPERTY(EditAnywhere)
	TMap<FString, bool> StoreListItemIDs;
	
	TArray<FOnlineStoreOfferRef> StoreOffers;
	
	UPROPERTY(EditAnywhere)
	FString LogInAchievementName;
	
	UPROPERTY(EditAnywhere)
	FString LogInAchievementID;
	
private:
	void OnLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
						  const FString& Error);
	
	void OnQueryAchievementsCompleted(const FUniqueNetId& UniqueNetId, bool bWasSuccessful);
	
	void OnQueryOnlineStoreOfferCompleted(bool bWasSuccessful, const TArray<FUniqueOfferId>& OfferIds, const FString& Error);
	
};
