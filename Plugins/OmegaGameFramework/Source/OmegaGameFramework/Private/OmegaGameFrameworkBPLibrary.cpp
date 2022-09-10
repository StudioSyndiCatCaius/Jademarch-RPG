// Copyright Studio Syndicat 2021. All Rights Reserved.


#include "OmegaGameFrameworkBPLibrary.h"
#include "OmegaGameFramework.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/GameplayTagsInterface.h"


TArray<UObject*> UOmegaGameFrameworkBPLibrary::FilterObjectsByCategoryTag(TArray<UObject*> Assets,
	FGameplayTag CategoryTag, bool bExact, bool bExclude, TSubclassOf<UObject> Class)
{
	TArray<UObject*> OutAssets;
	
	for(auto* TempAsset : Assets)
	{
		if(TempAsset)
		{
			if(!CategoryTag.IsValid() || (!Class || (TempAsset->GetClass()->IsChildOf(Class))) && TempAsset->Implements<UGameplayTagsInterface>())
			{
				FGameplayTag TempAssetTag = IGameplayTagsInterface::Execute_GetObjectGameplayCategory(TempAsset);
				bool bLocalIsValid;

				if(bExact)
				{
					bLocalIsValid = TempAssetTag.MatchesTagExact(CategoryTag);
				}
				else
				{
					bLocalIsValid = TempAssetTag.MatchesTag(CategoryTag);
				}
			
				if(bLocalIsValid != bExclude)
				{
					OutAssets.Add(TempAsset);
				}
			}
		}
	}
	return OutAssets;
}

TArray<UObject*> UOmegaGameFrameworkBPLibrary::FilterObjectsByGameplayTags(TArray<UObject*> Assets,
	FGameplayTagContainer GameplayTags, bool bExact, bool bExclude, TSubclassOf<UObject> Class)
{
	TArray<UObject*> OutAssets;
	
	for(auto* TempAsset : Assets)
	{
		if(TempAsset)
		{
			if(GameplayTags.IsEmpty() || (!Class || (TempAsset->GetClass()->IsChildOf(Class))) && TempAsset->Implements<UGameplayTagsInterface>())
			{
				FGameplayTagContainer TempAssetTag = IGameplayTagsInterface::Execute_GetObjectGameplayTags(TempAsset);
				bool bLocalIsValid;

				if(bExact)
				{
					bLocalIsValid = TempAssetTag.HasAnyExact(GameplayTags);
				}
				else
				{
					bLocalIsValid = TempAssetTag.HasAny(GameplayTags);
				}
			
				if(bLocalIsValid != bExclude)
				{
					OutAssets.Add(TempAsset);
				}
			}
		}
	}
	return OutAssets;
}

FGameplayTagContainer UOmegaGameFrameworkBPLibrary::FilterTagsByType(FGameplayTag TypeTag, FGameplayTagContainer TagsIn)
{
	FGameplayTagContainer OutTags;
	TArray<FGameplayTag> TempTags;
	TagsIn.GetGameplayTagArray(TempTags);
	for(FGameplayTag TempTag : TempTags)
	{
		if(TempTag.MatchesTag(TypeTag))
		{
			OutTags.AddTag(TempTag);
		}
	}
	return OutTags;
}

void UOmegaGameFrameworkBPLibrary::SetGameplaySystemActive(const UObject* WorldContextObject, TSubclassOf<AOmegaGameplaySystem>
	SystemClass, bool bActive, const FString Flag, UObject* Context)
{
	UOmegaGameplaySubsystem* SubystemRef = WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>();
	if(bActive)
	{
		SubystemRef->ActivateGameplaySystem(SystemClass, Context, Flag);
	}
	else
	{
		SubystemRef->ShutdownGameplaySystem(SystemClass, Flag);
	}
}

/*
void UOmegaGameFrameworkBPLibrary::SetWidgetVisibilityWithTags(FGameplayTagContainer Tags, ESlateVisibility Visibility)
{
	TArray<UUserWidget*> TempWidgets; 
	UWidgetBlueprintLibrary::GetAllWidgetsWithInterface(this, TempWidgets, UGameplayTagsInterface::StaticClass(), true);
	for(UUserWidget* TempWidget : TempWidgets)
	{
		TempWidget->SetVisibility(Visibility);
	}
}
*/
TArray<UObject*> UOmegaGameFrameworkBPLibrary::FilterObjectsByClass(TArray<UObject*> Objects, TSubclassOf<UObject> Class, bool bExclude)
{
	TArray<UObject*> OutObjects;
	for(auto* TempObj : Objects)
	{
		if(TempObj)
		{
			if(TempObj->GetClass()->IsChildOf(Class) && !bExclude)
			{
				OutObjects.Add(TempObj);
			}
		}
	}
	return OutObjects;
}

UObject* UOmegaGameFrameworkBPLibrary::SelectObjectByName(TArray<UObject*> Objects, const FString& Name)
{
	for(auto* TempObj : Objects)
	{
		if(TempObj && TempObj->GetName() == Name)
		{
			return TempObj;
		}
	}
	return nullptr;
}

AActor* UOmegaGameFrameworkBPLibrary::GetPlayerMouseOverActor(APlayerController* Player, ETraceTypeQuery TraceChannel, float TraceSphereRadius)
{
	FVector LocalStart;
	FVector LocalEnd;
	Player->DeprojectMousePositionToWorld(LocalStart, LocalEnd);
	LocalEnd = LocalEnd*Player->HitResultTraceDistance;

	FHitResult LocalHitResult;
	const TArray<AActor*> LocalIgnoreActors;
	if(UKismetSystemLibrary::SphereTraceSingle(Player, LocalStart, LocalEnd, TraceSphereRadius, TraceChannel, false, LocalIgnoreActors, EDrawDebugTrace::None, LocalHitResult, true))
	{
		if(LocalHitResult.GetActor())
		{
			return LocalHitResult.GetActor();
		}
	}
	
	return nullptr;
}

void UOmegaGameFrameworkBPLibrary::SetGlobalActorBinding(const UObject* WorldContextObject, FName Binding,
	AActor* Actor)
{
	WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>()->SetGlobalActorBinding(Binding, Actor);
}

void UOmegaGameFrameworkBPLibrary::ClearGlobalActorBinding(const UObject* WorldContextObject, FName Binding)
{
	WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>()->ClearGlobalActorBinding(Binding);
}

AActor* UOmegaGameFrameworkBPLibrary::GetGlobalActorBinding(const UObject* WorldContextObject, FName Binding)
{
	return WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>()->GetGlobalActorBinding(Binding);
}

