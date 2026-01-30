/*
* Copyright 2026 The Android Open Source Project
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       https://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#include "GameInstances/TrivialKartGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePurchaseInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/TrivialKartSaveGame.h"

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

void UTrivialKartGameInstance::AddAchievementProgress(const float Progress, const FString& AchievementName)
{
	if (AchievementName.IsEmpty())
		return;
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()); 
		IdentityInterface.IsValid())
	{
		if (const IOnlineAchievementsPtr AchievementsInterface = 
			Online::GetAchievementsInterface(GetWorld()); AchievementsInterface.IsValid())
		{
			FOnlineAchievement CurrentAchievement;
			AchievementsInterface->GetCachedAchievement(*IdentityInterface->GetUniquePlayerId(0), AchievementName, CurrentAchievement);
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

void UTrivialKartGameInstance::StartPurchasing(const FUniqueOfferId& PurchaseItemID, int32 Quantity, bool bIsConsumable)
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
								bIsConsumable
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

UTrivialKartSaveGame* UTrivialKartGameInstance::LoadGame()
{
	SaveGameInstance = Cast<UTrivialKartSaveGame>(UGameplayStatics::LoadGameFromSlot("TrivialKart", 0));
	if (!IsValid(SaveGameInstance))
	{
		SaveGameInstance = Cast<UTrivialKartSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameTemplate));
	}
	return SaveGameInstance;
}

void UTrivialKartGameInstance::SaveGame(UTrivialKartSaveGame* SaveData)
{
	if (!IsValid(SaveData))
		return;
	UGameplayStatics::SaveGameToSlot(SaveData, "TrivialKart", 0);
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
					StoreInterface->QueryOffersById(*IdentityInterface->GetUniquePlayerId(0), StoreListItemIDs, 
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
		AddAchievementProgress(20, LogInAchievementName);
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

