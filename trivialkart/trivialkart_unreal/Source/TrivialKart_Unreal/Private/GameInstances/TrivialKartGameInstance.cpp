#include "GameInstances/TrivialKartGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePurchaseInterface.h"

DEFINE_LOG_CATEGORY(LogTemplateGameInstance);

void UTrivialKartGameInstance::Init()
{
	Super::Init();
	InitiateAutoLogin();
}

void UTrivialKartGameInstance::InitiateAutoLogin()
{
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()))
	{
		IdentityInterface->AutoLogin(0);
		IdentityInterface->AddOnLoginCompleteDelegate_Handle(0, 
			FOnLoginCompleteDelegate::CreateUObject(this, 
				&UTrivialKartGameInstance::OnLoginCompleted));
	}
}

bool UTrivialKartGameInstance::GetLoginStatus() const
{
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()))
	{
		return IdentityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn;
	}
	return false;
}

FString UTrivialKartGameInstance::GetPlayerName() const
{
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()))
	{
		return IdentityInterface->GetPlayerNickname(0);
	}
	return FString();
}

void UTrivialKartGameInstance::AddAchievementProgress(const float Progress, const FString& AchievementName,
	const FString& AchievementID)
{
	if (AchievementName.IsEmpty() && AchievementID.IsEmpty())
		return;
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()); 
		IdentityInterface.IsValid())
	{
		if (const IOnlineAchievementsPtr AchievementsInterface = 
			Online::GetAchievementsInterface(GetWorld()); AchievementsInterface.IsValid())
		{
			FOnlineAchievement CurrentAchievement;
			AchievementsInterface->GetCachedAchievement(*IdentityInterface->GetUniquePlayerId(0),AchievementID, CurrentAchievement);
			if (CurrentAchievement.Progress < 100.0)
			{
				const float CurrentProgress = CurrentAchievement.Progress + Progress;
				const FOnlineAchievementsWritePtr AchievementPtr = MakeShareable(new FOnlineAchievementsWrite());
				AchievementPtr->SetFloatStat(AchievementName, CurrentProgress);
				FOnlineAchievementsWriteRef AchievementRef = AchievementPtr.ToSharedRef();
				AchievementsInterface->WriteAchievements(*IdentityInterface->GetUniquePlayerId(0),
					AchievementRef);
			}
		}
	}
}

void UTrivialKartGameInstance::StartPurchasing(const FUniqueOfferId& PurchaseItemID, int32 Quantity)
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
		{
			if (const IOnlineStoreV2Ptr StoreV2Interface = Subsystem->GetStoreV2Interface())
			{
				if (const TSharedPtr<FOnlineStoreOffer> CurrentOrder = StoreV2Interface->GetOffer(PurchaseItemID); 
					CurrentOrder.IsValid())
				{
					if (const IOnlinePurchasePtr PurchaseInterface =
						Subsystem->GetPurchaseInterface(); PurchaseInterface.IsValid())
					{
						if (PurchaseInterface->IsAllowedToPurchase(*IdentityInterface->GetUniquePlayerId(0)))
						{
							FPurchaseCheckoutRequest CheckoutRequest;

							// Use the product ID from the Google Play Console (OfferId).
							CheckoutRequest.AddPurchaseOffer(
								"", 
								CurrentOrder->OfferId,
								Quantity,
								StoreListItemIDs[CurrentOrder->OfferId]
							);
							PurchaseInterface->Checkout(*IdentityInterface->GetUniquePlayerId(0),
								CheckoutRequest,
								FOnPurchaseCheckoutComplete::CreateWeakLambda(this, 
									[this, Subsystem, IdentityInterface, PurchaseItemID, Quantity]
									(const FOnlineError& OnlineError, const TSharedRef<FPurchaseReceipt>& PurchaseReceipt)
								{
									if (!OnlineError.WasSuccessful())
										return;
									if (OnPurchaseReceived.IsBound())
									{
										OnPurchaseReceived.Broadcast(PurchaseItemID, Quantity);
									}
									// The Purchase Token is passed as the ReceiptId to tell the platform which purchase to finalize (consume/acknowledge).
									Subsystem->GetPurchaseInterface()->FinalizePurchase(*IdentityInterface->GetUniquePlayerId(0), PurchaseReceipt->TransactionId);
								}));
						}
					}
				}
			}
		}
	}
}

void UTrivialKartGameInstance::OnLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
                                                const FString& Error)
{
UE_LOG(LogTemplateGameInstance, Log, TEXT("Local User: %d of Unique ID: %s has logged in with Status: %s"), 
		LocalUserNum, *UserId.ToDebugString(), bWasSuccessful ? TEXT("Success") : *Error);
	if (bWasSuccessful)
	{
		if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
		{
			if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
			{
				if (const IOnlineAchievementsPtr AchievementsInterface =
					Subsystem->GetAchievementsInterface())
				{
					AchievementsInterface->QueryAchievements(*IdentityInterface->GetUniquePlayerId(0),
						FOnQueryAchievementsCompleteDelegate::CreateUObject(this,
							&UTrivialKartGameInstance::OnQueryAchievementsCompleted));
				}
				if (const IOnlineStoreV2Ptr StoreInterface = Subsystem->GetStoreV2Interface())
				{
					TArray<FString> StoreListItemKeys;
					StoreListItemIDs.GetKeys(StoreListItemKeys);
					StoreInterface->QueryOffersById(*IdentityInterface->GetUniquePlayerId(0), StoreListItemKeys, 
						FOnQueryOnlineStoreOffersComplete::CreateUObject(this, &UTrivialKartGameInstance::OnQueryOnlineStoreOfferCompleted));
				}
			}
		}
	}
}

void UTrivialKartGameInstance::OnQueryAchievementsCompleted(const FUniqueNetId& UniqueNetId, bool bWasSuccessful)
{
UE_LOG(LogTemplateGameInstance, Log, TEXT("Achievements Cached for User: %s has %s"), *UniqueNetId.ToString(), bWasSuccessful ? TEXT("Succeeded") : TEXT("Failed"));
	if (bWasSuccessful)
	{
		AddAchievementProgress(20, LogInAchievementID, LogInAchievementName);
	}
}

void UTrivialKartGameInstance::OnQueryOnlineStoreOfferCompleted(bool bWasSuccessful,
	const TArray<FUniqueOfferId>& OfferIds, const FString& Error)
{
	if (bWasSuccessful)
	{
		if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
		{
			if (const IOnlineStoreV2Ptr StoreInterface = Subsystem->GetStoreV2Interface())
			{
				StoreInterface->GetOffers(StoreOffers);
			}
		}
	}
}

