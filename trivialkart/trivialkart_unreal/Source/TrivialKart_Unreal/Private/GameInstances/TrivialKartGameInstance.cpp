#include "GameInstances/TrivialKartGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"

DEFINE_LOG_CATEGORY(LogTemplateGameInstance);

void UTrivialKartGameInstance::Init()
{
	Super::Init();
	InitiateAutoLogin();
}

void UTrivialKartGameInstance::InitiateAutoLogin()
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
		{
			IdentityInterface->AutoLogin(0);
			IdentityInterface->AddOnLoginCompleteDelegate_Handle(0, 
				FOnLoginCompleteDelegate::CreateUObject(this, 
					&UTrivialKartGameInstance::OnLoginCompleted));
		}
	}
}

bool UTrivialKartGameInstance::GetLoginStatus() const
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
		{
			return IdentityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn;
		}
	}
	return false;
}

FString UTrivialKartGameInstance::GetPlayerName() const
{
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineIdentityPtr IdentityInterface = Subsystem->GetIdentityInterface())
		{
			return IdentityInterface->GetPlayerNickname(0);
		}
	}
	return FString();
}

void UTrivialKartGameInstance::OnLoginCompleted(int LocalUserNum, bool bWasSuccessful, 
	const FUniqueNetId& UserId, const FString& Error) const
{
	UE_LOG(LogTemplateGameInstance, Log, TEXT("Local User: %d of Unique ID: %s has logged in with Status: %s"), 
		LocalUserNum, *UserId.ToDebugString(), bWasSuccessful ? *Error : TEXT("Success"));
}
