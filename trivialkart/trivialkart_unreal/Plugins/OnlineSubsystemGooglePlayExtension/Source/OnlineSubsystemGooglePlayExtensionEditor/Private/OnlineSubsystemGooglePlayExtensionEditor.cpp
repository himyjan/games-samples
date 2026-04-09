#include "OnlineSubsystemGooglePlayExtensionEditor.h"

#include "ISettingsModule.h"
#include "OnlineSubsystemGooglePlayExtensionRuntimeSettings.h"

#define LOCTEXT_NAMESPACE "FOnlineSubsystemGooglePlayExtensionEditorModule"

void FOnlineSubsystemGooglePlayExtensionEditorModule::StartupModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)	
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "OnlineSubsystemGooglePlayExtension",
			LOCTEXT("OnlineSubsystemGooglePlayExtensionSettingsName", "Online Subsystem Google Play Extension"),
			LOCTEXT("OnlineSubsystemGooglePlayExtensionSettingsDescription", "Project settings for Online Subsystem Google Play Extension plugin"),
			GetMutableDefault<UOnlineSubsystemGooglePlayExtensionRuntimeSettings>()
		);
	}
}

void FOnlineSubsystemGooglePlayExtensionEditorModule::ShutdownModule()
{
	// unregister settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "OnlineSubsystemGooglePlayExtension");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FOnlineSubsystemGooglePlayExtensionEditorModule, OnlineSubsystemGooglePlayExtensionEditor)