// Copyright Studio Syndicat 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OmegaGameplaySubsystem.h"
#include "GameplayTagContainer.h"
#include "Gameplay/OmegaGameplayModule.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OmegaInputMode.h"
#include "OmegaGameFrameworkBPLibrary.generated.h"

class APlayerController;
class UEnhancedInputLocalPlayerSubsystem;
class UOmegaPlayerSubsystem;


UCLASS()
class UOmegaGameFrameworkBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/*
	UFUNCTION(BlueprintPure, Category = "Ω|Gameplay", meta = (DeterminesOutputType = "Class", CompactNodeTitle = "Gameplay System"))
	static AOmegaGameplaySystem* GetGameplaySystem(class TSubclassOf<AOmegaGameplaySystem> Class, bool& bIsActive);

	*/
	//Exact: Exact tag? | Exclude: if true, will exclude matching objects instead of including them.
	UFUNCTION(BlueprintPure, Category = "Ω|Assets", meta=(AdvancedDisplay="bExact, bExclude, bInvertCheck, Class", DeterminesOutputType="Class"))
	static TArray<UObject*> FilterObjectsByCategoryTag(TArray<UObject*> Assets, FGameplayTag CategoryTag,
		bool bExact, bool bExclude, TSubclassOf<UObject> Class);

	//Exact: Exact tag? | Exclude: if true, will exclude matching objects instead of including them.
	UFUNCTION(BlueprintPure, Category = "Ω|Assets", meta=(AdvancedDisplay="bExact, bExclude, bInvertCheck, Class", DeterminesOutputType="Class"))
	static TArray<UObject*> FilterObjectsByGameplayTags(TArray<UObject*> Assets, FGameplayTagContainer GameplayTags,
		bool bExact, bool bExclude, TSubclassOf<UObject> Class);

	UFUNCTION(BlueprintPure, Category="Ω|GameplayTags")
	static FGameplayTagContainer FilterTagsByType(FGameplayTag TypeTag, FGameplayTagContainer TagsIn);
/*
	UFUNCTION(BlueprintCallable, Category="Ω|Widgets")
	static void SetWidgetVisibilityWithTags(FGameplayTagContainer Tags, ESlateVisibility Visibility);
*/

	UFUNCTION(BlueprintCallable, Category="Combat", meta = (WorldContext = "WorldContextObject", AdvancedDisplay="Flag, Context")) 
	static void SetGameplaySystemActive(const UObject* WorldContextObject, TSubclassOf<AOmegaGameplaySystem> SystemClass, bool bActive, const FString Flag, UObject* Context);

	
	UFUNCTION(BlueprintPure, Category="Ω|Assets", meta=(DeterminesOutputType="Class", AdvancedDisplay="bExclude"))
	static TArray<UObject*> FilterObjectsByClass(TArray<UObject*> Objects, TSubclassOf<UObject> Class, bool bExclude);

	UFUNCTION(BlueprintPure, Category="Ω|Assets", meta=(DeterminesOutputType="Class", AdvancedDisplay="bExclude"))
	static UObject* SelectObjectByName(TArray<UObject*> Objects, const FString& Name);

	//PLAYER
	UFUNCTION(BlueprintPure, Category="Ω|Player")
	static AActor* GetPlayerMouseOverActor(APlayerController* Player, ETraceTypeQuery TraceChannel, float TraceSphereRadius);

	UFUNCTION(BlueprintCallable, Category="Ω|Player", meta = (WorldContext = "WorldContextObject", AdvancedDisplay="Player"))
	static void SetPlayerCustomInputMode(const UObject* WorldContextObject, APlayerController* Player,  UOmegaInputMode* InputMode);

	//------- ACTOR BINDING --------//
	
	UFUNCTION(BlueprintCallable, Category="Omega Gameplay", meta = (WorldContext = "WorldContextObject"))
	static void SetGlobalActorBinding(const UObject* WorldContextObject, FName Binding, AActor* Actor);
	UFUNCTION(BlueprintCallable, Category="Omega Gameplay", meta = (WorldContext = "WorldContextObject"))
	static void ClearGlobalActorBinding(const UObject* WorldContextObject, FName Binding);
	UFUNCTION(BlueprintPure, Category="Omega Gameplay", meta = (WorldContext = "WorldContextObject"))
	static AActor* GetGlobalActorBinding(const UObject* WorldContextObject, FName Binding);

	//------- QuickGet --------//

	//Get an Active Gameplay System by Class
	UFUNCTION(BlueprintPure, Category="Omega Gameplay", meta = (WorldContext = "WorldContextObject", DeterminesOutputType="SystemClass", CompactNodeTitle="Gameplay System"))
	static AOmegaGameplaySystem* GetActiveGameplaySystem(const UObject* WorldContextObject, TSubclassOf<AOmegaGameplaySystem> SystemClass);
	
	//Get an Active Gameplay Module by Class
	UFUNCTION(BlueprintPure, Category="Omega Gameplay", meta = (WorldContext = "WorldContextObject", DeterminesOutputType="ModuleClass", CompactNodeTitle="Gameplay Module"))
	static UOmegaGameplayModule* GetGameplayModule(const UObject* WorldContextObject, TSubclassOf<UOmegaGameplayModule> ModuleClass);
};
