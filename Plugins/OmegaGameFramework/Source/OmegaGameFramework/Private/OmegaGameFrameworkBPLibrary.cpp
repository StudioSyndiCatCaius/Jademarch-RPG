// Copyright Studio Syndicat 2021. All Rights Reserved.


#include "OmegaGameFrameworkBPLibrary.h"
#include "OmegaGameFramework.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "OmegaGameManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/GameplayTagsInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/OmegaPlayerSubsystem.h"


bool UOmegaGameFrameworkBPLibrary::IsObjectOfGameplayCategory(UObject* Object, FGameplayTag CategoryTag, bool bExact)
{
	if(Object && Object->GetClass()->ImplementsInterface(UGameplayTagsInterface::StaticClass()))
	{
		const FGameplayTag LocalCategory = IGameplayTagsInterface::Execute_GetObjectGameplayCategory(Object);
		if(bExact)
		{
			return LocalCategory.MatchesTagExact(CategoryTag);
		}
		return LocalCategory.MatchesTag(CategoryTag);
		
	}
	return false;
}

bool UOmegaGameFrameworkBPLibrary::DoesObjectHaveGameplayTag(UObject* Object, FGameplayTag GameplayTag, bool bExact)
{
	if(Object && Object->GetClass()->ImplementsInterface(UGameplayTagsInterface::StaticClass()))
	{
		const FGameplayTagContainer LocalTags = IGameplayTagsInterface::Execute_GetObjectGameplayTags(Object);
		if(bExact)
		{
			return LocalTags.HasTagExact(GameplayTag);
		}
		return LocalTags.HasTag(GameplayTag);
	}
	return false;
}

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

FGameplayTag UOmegaGameFrameworkBPLibrary::GetFirstTagOfType(FGameplayTag TypeTag, FGameplayTagContainer TagsIn)
{
	TArray<FGameplayTag> LocalTags;
	FilterTagsByType(TypeTag, TagsIn).GetGameplayTagArray(LocalTags);
	FGameplayTag OutTag;
	if(LocalTags.IsValidIndex(0))
	{
		OutTag = LocalTags[0];
	}
	return OutTag;
}

void UOmegaGameFrameworkBPLibrary::SetGameplaySystemActive(const UObject* WorldContextObject, TSubclassOf<AOmegaGameplaySystem>
                                                           SystemClass, bool bActive, const FString Flag, UObject* Context)
{
	UObject* LocalContext = nullptr;
	if(Context)
	{
		LocalContext = Context;
	}
	UOmegaGameplaySubsystem* SubystemRef = WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>();
	if(bActive)
	{
		SubystemRef->ActivateGameplaySystem(SystemClass, LocalContext, Flag);
	}
	else
	{
		SubystemRef->ShutdownGameplaySystem(SystemClass, LocalContext, Flag);
	}
}

void UOmegaGameFrameworkBPLibrary::SetGameplaySystemsActive(const UObject* WorldContextObject,
	TArray<TSubclassOf<AOmegaGameplaySystem>> SystemClasses, bool bActive, const FString Flag, UObject* Context)
{
	for(auto TempClass : SystemClasses)
	{
		if(TempClass)
		{
			SetGameplaySystemActive(WorldContextObject, TempClass, bActive, Flag, Context);
		}
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

void UOmegaGameFrameworkBPLibrary::SetPlayerCustomInputMode(const UObject* WorldContextObject,
	APlayerController* Player, UOmegaInputMode* InputMode)
{
	if(!InputMode)
	{
		return;
	}
	const APlayerController* TempPlayer = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if(Player)
	{
		TempPlayer = Player;
	}
	TempPlayer->GetLocalPlayer()->GetSubsystem<UOmegaPlayerSubsystem>()->SetCustomInputMode(InputMode);
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

// QUICK ACCESS
AOmegaGameplaySystem* UOmegaGameFrameworkBPLibrary::GetActiveGameplaySystem(const UObject* WorldContextObject,
	TSubclassOf<AOmegaGameplaySystem> SystemClass)
{
	bool LocalIsActive;
	if(WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>()->GetGameplaySystem(SystemClass, LocalIsActive))
	{
		return WorldContextObject->GetWorld()->GetSubsystem<UOmegaGameplaySubsystem>()->GetGameplaySystem(SystemClass, LocalIsActive);
	}
	return nullptr;
}

UOmegaGameplayModule* UOmegaGameFrameworkBPLibrary::GetGameplayModule(const UObject* WorldContextObject,
	TSubclassOf<UOmegaGameplayModule> ModuleClass)
{
	if(WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>()->GetGameplayModule(ModuleClass))
	{
		return WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>()->GetGameplayModule(ModuleClass);
	}
	return nullptr;
}

void UOmegaGameFrameworkBPLibrary::FireGlobalEvent(const UObject* WorldContextObject, FName Event, UObject* Context)
{
	WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>()->FireGlobalEvent(Event, Context);
}

void UOmegaGameFrameworkBPLibrary::OnFlagActiveReset(const UObject* WorldContextObject, const FString& Flag, bool bDeactivateFlagOnActive, TEnumAsByte<EOmegaFlagResult>& Outcome)
{
	UOmegaGameManager* LocalMod = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>();
	if(LocalMod->IsFlagActive(Flag))
	{
		Outcome = EOmegaFlagResult::Flag_Active;
		if(bDeactivateFlagOnActive)
		{
			LocalMod->SetFlagActive(Flag, false);
		}
	}
	else
	{
		Outcome = EOmegaFlagResult::Flag_Inactive;
	}
}

void UOmegaGameFrameworkBPLibrary::SetActorTagActive(AActor* Actor, FName Tag, bool bIsActive)
{
	if(Actor)
	{
		if(bIsActive)
		{
			Actor->Tags.AddUnique(Tag);
		}
		else
		{
			Actor->Tags.Remove(Tag);
		}
	}
}

void UOmegaGameFrameworkBPLibrary::SetComponentTagActive(UActorComponent* Component, FName Tag, bool bIsActive)
{
	if(Component)
	{
		if(bIsActive)
		{
			Component->ComponentTags.AddUnique(Tag);
		}
		else
		{
			Component->ComponentTags.Remove(Tag);
		}
	}
}

TArray<AActor*> UOmegaGameFrameworkBPLibrary::GetActorsFromComponents(TArray<UActorComponent*> Components)
{
	TArray<AActor*> OutActors;
	for(const auto* TempComp : Components)
	{
		if(TempComp)
		{
			OutActors.AddUnique(TempComp->GetOwner());
		}
	}
	return OutActors;
}

TArray<UActorComponent*> UOmegaGameFrameworkBPLibrary::GetComponentsFromActors(TArray<AActor*> Actors, TSubclassOf<UActorComponent> ComponentClass)
{
	TArray<UActorComponent*> OutComps;
	for(const auto* TempComp : Actors)
	{
		if(TempComp && TempComp->GetComponentByClass(ComponentClass))
		{
			OutComps.AddUnique(TempComp->GetComponentByClass(ComponentClass));
		}
	}
	return OutComps;
}

AActor* UOmegaGameFrameworkBPLibrary::GetClosestActorToPoint(TArray<AActor*> Actors, FVector Point)
{
	AActor* OutActor = nullptr;
	
	for(auto* TempActor : Actors)
	{
		if(OutActor)
		{
			const float CheckedActor_Point = UKismetMathLibrary::Vector_Distance(Point, TempActor->GetActorLocation());
			const float OutActor_Point = UKismetMathLibrary::Vector_Distance(Point, OutActor->GetActorLocation());
			if(CheckedActor_Point<OutActor_Point)	//Checked actor distance is less than last actopr
			{
				OutActor = TempActor;
			}
		}
		else
		{
			OutActor = TempActor;
		}
	}
	return OutActor;
}

void UOmegaGameFrameworkBPLibrary::SetActorActive(AActor* Actor, bool bIsActive)
{
	if(Actor)
	{
		Actor->SetActorHiddenInGame(!bIsActive);
		Actor->CustomTimeDilation=bIsActive;
		Actor->SetActorEnableCollision(bIsActive);
	}
}

TArray<AActor*> UOmegaGameFrameworkBPLibrary::FilterActorsWithComponents(TArray<AActor*> Actors,
	TSubclassOf<UActorComponent> ComponentClass, TArray<AActor*> ExcludedActors)
{
	TArray<AActor*> OutActors;
	for(auto* TempActor : Actors)
	{
		if(TempActor && TempActor->GetComponentByClass(ComponentClass) && !ExcludedActors.Contains(TempActor))
		{
			OutActors.Add(TempActor);
		}
	}
	return OutActors;
}

UActorComponent* UOmegaGameFrameworkBPLibrary::TryGetComponentFromObject(UObject* Object,
	TSubclassOf<UActorComponent> Class, TEnumAsByte<EOmegaFunctionResult>& Outcome)
{
	if(!Object || !Class)
	{
		Outcome = EOmegaFunctionResult::Fail;
		return nullptr;
	}
	
	if (Cast<AActor>(Object) && Cast<AActor>(Object)->GetComponentByClass(Class))
	{
		Outcome = EOmegaFunctionResult::Success;
		return Cast<AActor>(Object)->GetComponentByClass(Class);
	}
	
	if (Cast<UActorComponent>(Object))
	{
		UActorComponent* TempComp = Cast<UActorComponent>(Object);
		
		if(TempComp->GetClass()->IsChildOf(Class))
		{
			Outcome = EOmegaFunctionResult::Success;
			return TempComp;
		}
		
		if (TempComp->GetOwner()->GetComponentByClass(Class))
		{
			Outcome = EOmegaFunctionResult::Success;
			return TempComp->GetOwner()->GetComponentByClass(Class);
		}
	}
	Outcome = EOmegaFunctionResult::Fail;
	return  nullptr;
}

//###############################################################################
// InterpActor
//###############################################################################

void UOmegaGameFrameworkBPLibrary::InterpActorLocation(AActor* Actor, FVector TargetLocation, float InterpSpeed)
{
	if(Actor)
	{
		Actor->SetActorLocation(UKismetMathLibrary::VInterpTo(Actor->GetActorLocation(), TargetLocation, UGameplayStatics::GetWorldDeltaSeconds(Actor), InterpSpeed));
	}
}

void UOmegaGameFrameworkBPLibrary::InterpActorRotation(AActor* Actor, FRotator TargetRotation, float InterpSpeed, bool X,
	bool Y, bool Z)
{
	FRotator LocalRot = TargetRotation;
	if(!X)
	{
		LocalRot.Roll = 0;
	}
	if(!Y)
	{
		LocalRot.Pitch = 0;
	}
	if(!Z)
	{
		LocalRot.Yaw = 0;
	}
	if(Actor)
	{
		Actor->SetActorRotation(UKismetMathLibrary::RInterpTo(Actor->GetActorRotation(), LocalRot, UGameplayStatics::GetWorldDeltaSeconds(Actor), InterpSpeed));
	}
	
}

