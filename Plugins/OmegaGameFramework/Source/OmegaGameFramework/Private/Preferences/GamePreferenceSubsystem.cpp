// Copyright Studio Syndicat 2021. All Rights Reserved.


#include "Preferences/GamePreferenceSubsystem.h"

#include "OmegaGameFrameworkBPLibrary.h"
#include "OmegaSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Save/OmegaSaveSubsystem.h"
#include "Save/OmegaSaveGlobal.h"
#include "Save/OmegaSaveGame.h"
#include "Engine/GameInstance.h"


#include "Preferences/Asset/GamePreferenceBool.h"
#include "Preferences/Asset/GamePreferenceFloat.h"
#include "Preferences/Asset/GamePreferenceInt.h"
#include "Preferences/Asset/GamePreferenceString.h"
#include "Preferences/Asset/GamePreferenceTag.h"

void UGamePreferenceSubsystem::PreloadPrefs()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	Filter.ClassNames.Add("GamePreferences");
	
	for(const FDirectoryPath NewPath : GetMutableDefault<UOmegaSettings>()->Preferences_ScanPaths)
	{
		FString LocalString = NewPath.Path;
		Filter.PackagePaths.Add(FName(*LocalString));
	}
	
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);

	for(FAssetData TempAssetData : AssetData)
	{
		if(Cast<UGamePreference>(TempAssetData.GetAsset()))
		{
			PreloadedPreferences.AddUnique(Cast<UGamePreference>(TempAssetData.GetAsset()));
		}
	}
}

void UGamePreferenceSubsystem::Initialize(FSubsystemCollectionBase& Colection)
{
	PreloadPrefs();
}

//BOOL////////////////////////

TArray<UGamePreference*> UGamePreferenceSubsystem::GetAllGamePreferences()
{
	return PreloadedPreferences;
}

UOmegaSaveSubsystem* UGamePreferenceSubsystem::GetSaveSubsystem()
{
	return UGameplayStatics::GetGameInstance(this)->GetSubsystem<UOmegaSaveSubsystem>();
}

void UGamePreferenceSubsystem::Local_PreferenceUpdate(UGamePreference* Preference)
{
	if(Preference)
	{
		//Preference->PreferenceScript->OnPreferenceValueUpdated(Preference, this, 0);
	}
}

void UGamePreferenceSubsystem::Local_PreferenceUpdateAll()
{
	 for(auto* TempPref : PreloadedPreferences)
	 {
		 Local_PreferenceUpdate(TempPref);
	 }
}

bool UGamePreferenceSubsystem::GetGamePreferenceBool(UGamePreferenceBool* Preference)
{
	if(!Preference)
	{
		return false;
	}
	
	TMap<UGamePreferenceBool*, bool> LocalBoolPrefs;
	
	if (Preference->bGlobal)
	{
		LocalBoolPrefs = GetSaveSubsystem()->GlobalSaveData->BoolPrefs;
	}
	else
	{
		LocalBoolPrefs = GetSaveSubsystem()->ActiveSaveData->BoolPrefs;
	}
	
	if (LocalBoolPrefs.Contains(Preference))
	{
		return LocalBoolPrefs[Preference];
	}
		return Preference->bDefaultValue;
}

void UGamePreferenceSubsystem::SetGamePreferenceBool(UGamePreferenceBool* Preference, bool bValue)
{
	if(Preference)
	{
		if (Preference->bGlobal)
		{
			GetSaveSubsystem()->GlobalSaveData->BoolPrefs.Add(Preference, bValue);
		}
		else
		{
			GetSaveSubsystem()->ActiveSaveData->BoolPrefs.Add(Preference, bValue);
		}
		OnBoolPreferenceUpdated.Broadcast(Preference, bValue);
	}
	Local_PreferenceUpdate(Preference);
}

//FLOAT////////////////////////

float UGamePreferenceSubsystem::GetGamePreferenceFloat(UGamePreferenceFloat* Preference)
{
	if(!Preference)
	{
		return 0.0f;
	}
	
	TMap<UGamePreferenceFloat*, float> FloatPrefsList;

	if (Preference->bGlobal)
	{
		FloatPrefsList = GetSaveSubsystem()->GlobalSaveData->FloatPrefs;
	}
	else
	{
		FloatPrefsList = GetSaveSubsystem()->ActiveSaveData->FloatPrefs;
	}

	if (FloatPrefsList.Contains(Preference))
	{
		return FloatPrefsList[Preference];
	}
		return Preference->DefaultValue;
}

void UGamePreferenceSubsystem::SetGamePreferenceFloat(UGamePreferenceFloat* Preference, float Value)
{
	if (Preference)
	{
		if (Preference->bGlobal)
		{
			GetSaveSubsystem()->GlobalSaveData->FloatPrefs.Add(Preference, Value);
		}
		else
		{
			GetSaveSubsystem()->ActiveSaveData->FloatPrefs.Add(Preference, Value);
		}
		OnFloatPreferenceUpdated.Broadcast(Preference, Value);
	}
	Local_PreferenceUpdate(Preference);
}

//Int////////////////////////

int32 UGamePreferenceSubsystem::GetGamePreferenceInt(UGamePreferenceInt* Preference)
{
	if(!Preference)
	{
		return 0;
	}
	
	TMap<UGamePreferenceFloat*, float> IntPr;

	if (Preference->bGlobal)
	{
		IntPrefsList = GetSaveSubsystem()->GlobalSaveData->IntPrefs;
	}
	else
	{
		IntPrefsList = GetSaveSubsystem()->ActiveSaveData->IntPrefs;
	}

	if (IntPrefsList.Contains(Preference))
	{
		return IntPrefsList[Preference];
	}
	return Preference->DefaultValue;
}

void UGamePreferenceSubsystem::SetGamePreferenceInt( UGamePreferenceInt* Preference, int32 Value)
{
	if (Preference)
	{
		if (Preference->bGlobal)
		{
			GetSaveSubsystem()->GlobalSaveData->IntPrefs.Add(Preference, Value);
		}
		else
		{
			GetSaveSubsystem()->ActiveSaveData->IntPrefs.Add(Preference, Value);
		}
		OnIntPreferenceUpdated.Broadcast(Preference, Value);
	}
	Local_PreferenceUpdate(Preference);
}

//STRING////////////////////////

FString UGamePreferenceSubsystem::GetGamePreferenceString(UGamePreferenceString* Preference)
{
	if(!Preference)
	{
		return "";
	}
	TMap<UGamePreferenceString*, FString> PrefsList;

	if (Preference->bGlobal)
	{
		PrefsList = GetSaveSubsystem()->GlobalSaveData->StringPrefs;
	}
	else
	{
		PrefsList = GetSaveSubsystem()->ActiveSaveData->StringPrefs;
	}
	TMap<int32, float> DumMap;


	if (PrefsList.Contains(Preference))
	{
		return PrefsList[Preference];
	}

		return Preference->DefaultValue;

}

void UGamePreferenceSubsystem::SetGamePreferenceString(UGamePreferenceString* Preference, const FString& Value)
{
	if (Preference)
	{
		if (Preference->bGlobal)
		{
			GetSaveSubsystem()->GlobalSaveData->StringPrefs.Add(Preference, Value);
		}
		else
		{
			GetSaveSubsystem()->ActiveSaveData->StringPrefs.Add(Preference, Value);
		}
		OnStringPreferenceUpdated.Broadcast(Preference, Value);
	}
	Local_PreferenceUpdate(Preference);
}

//TAG////////////////////////

FGameplayTag UGamePreferenceSubsystem::GetGamePreferenceTag(UGamePreferenceTag* Preference)
{
	if (!Preference)
	{
		const FGameplayTag BadTags;
		return BadTags; 
	}
	
	TMap<UGamePreferenceTag*, FGameplayTag> PrefsList;

	if (Preference->bGlobal)
	{
		PrefsList = GetSaveSubsystem()->GlobalSaveData->TagPrefs;
	}
	else
	{
		PrefsList = GetSaveSubsystem()->ActiveSaveData->TagPrefs;
	}
	TMap<int32, float> DumMap;


	if (PrefsList.Contains(Preference))
	{
		return PrefsList[Preference];
	}

		return Preference->DefaultValue;
	
}

void UGamePreferenceSubsystem::SetGamePreferenceTag(UGamePreferenceTag* Preference, FGameplayTag Value)
{
	if (Preference)
	{
		if (Preference->bGlobal)
		{
			GetSaveSubsystem()->GlobalSaveData->TagPrefs.Add(Preference, Value);
			
		}
		else
		{
			GetSaveSubsystem()->ActiveSaveData->TagPrefs.Add(Preference, Value);
		}
		OnTagPreferenceUpdated.Broadcast(Preference, Value);
	}
	Local_PreferenceUpdate(Preference);
}
