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
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()))
	{
		IdentityInterface->AddOnLoginCompleteDelegate_Handle(0,
			FOnLoginCompleteDelegate::CreateUObject(this,
				&UTrivialKartGameInstance::OnLoginCompleted));
	}
	if (const IOnlineUserCloudPtr CloudInterface = Online::GetUserCloudInterface(GetWorld()))
	{
		CloudInterface->AddOnReadUserFileCompleteDelegate_Handle(
			FOnReadUserFileCompleteDelegate::CreateUObject(this, &UTrivialKartGameInstance::OnCloudReadComplete));
	}
	InitiateAutoLogin();
}

void UTrivialKartGameInstance::InitiateAutoLogin()
{
	if (const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld()))
	{
		IdentityInterface->AutoLogin(0);
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

void UTrivialKartGameInstance::StartPurchasing(const FString& OfferID, const int32 Quantity, bool bIsConsumable)
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
		{
			if (const IOnlineStoreV2Ptr StoreV2Interface = Subsystem->GetStoreV2Interface())
			{
				if (const TSharedPtr<FOnlineStoreOffer> CurrentOrder = StoreV2Interface->GetOffer(OfferID);
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
									[this, Subsystem, IdentityInterface, OfferID, Quantity]
									(const FOnlineError& OnlineError, const TSharedRef<FPurchaseReceipt>& PurchaseReceipt)
								{
									if (!OnlineError.WasSuccessful())
										return;
									if (OnPurchaseReceived.IsBound())
									{
										OnPurchaseReceived.Broadcast(OfferID, Quantity);
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

	// 1. Save locally as before
	UGameplayStatics::SaveGameToSlot(SaveData, "TrivialKart", 0);

	// 2. Synchronize to Google Play Cloud
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
		{
			if (const IOnlineUserCloudPtr CloudInterface = Subsystem->GetUserCloudInterface())
			{
				TArray<uint8> ObjectBytes;
				UGameplayStatics::SaveGameToMemory(SaveData, ObjectBytes);

				CloudInterface->WriteUserFile(*IdentityInterface->GetUniquePlayerId(0), "TrivialKartCloudSave", ObjectBytes);
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
					StoreInterface->QueryOffersById(*IdentityInterface->GetUniquePlayerId(0), StoreListItemIDs, 
						FOnQueryOnlineStoreOffersComplete::CreateUObject(this, &UTrivialKartGameInstance::OnQueryOnlineStoreOfferCompleted));
				}
				if (const IOnlineUserCloudPtr CloudInterface = Subsystem->GetUserCloudInterface())
				{
					CloudInterface->AddOnReadUserFileCompleteDelegate_Handle(
						FOnReadUserFileCompleteDelegate::CreateUObject(this, &UTrivialKartGameInstance::OnCloudReadComplete));

					CloudInterface->ReadUserFile(*IdentityInterface->GetUniquePlayerId(0), "TrivialKartCloudSave");
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

void UTrivialKartGameInstance::OnCloudReadComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName)
{
	if (bWasSuccessful)
	{
		if (const IOnlineUserCloudPtr CloudInterface = Online::GetUserCloudInterface(GetWorld()))
		{
			TArray<uint8> FileContents;
			if (CloudInterface->GetFileContents(UserId, FileName, FileContents))
			{
				// Deserialize the bytes back into your SaveGame object
				UTrivialKartSaveGame* CloudData = Cast<UTrivialKartSaveGame>(UGameplayStatics::LoadGameFromMemory(FileContents));

				// Update your current instance and save locally to keep them in sync
				SaveGameInstance = CloudData;

				FString CarName;

				if (IsValid(SaveGameInstance))
				{
					CarName = SaveGameInstance->ActiveCarType;
				}

				UE_LOG(LogTemplateGameInstance, Log, TEXT("Cloud Save successfully synced: %s and Car : %s"), *FileName, *CarName);
			}
		}
	}
}

