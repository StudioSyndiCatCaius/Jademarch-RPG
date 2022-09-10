// Copyright Studio Syndicat 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/GameplayTagsInterface.h"
#include "GameplayTagContainer.h"
#include "VolumeUtils.h"
#include "DamageFormula.h"
#include "Gameplay/Combatant/OmegaEffectPopup.h"
#include "OmegaGameplayEffect.generated.h"

class AOmegaGameplayEffect;


USTRUCT(BlueprintType)
struct FOmegaEffectContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	TSubclassOf<AOmegaGameplayEffect> EffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	float Power = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FGameplayTagContainer AddedTags;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectTriggered, AOmegaGameplayEffect*, Effect, float, DamageValue);


UENUM()
enum class EEffectLifetime : uint8

{
	// Effect is instantly triggered and destruct
	EffectLifetime_Instant        UMETA(DisplayName = "Instant"),
	//Effect will trigger and destruct after a certain amount of timer has passed.
	EffectLifetime_Timer        UMETA(DisplayName = "Timer"),
	// Effect remains until Trigger
	EffectLifetime_OnTrigger        UMETA(DisplayName = "OnTrigger"),
	// Effect remains until "Destroy Actor" is called.
	EffectLifetime_OnDestroy       UMETA(DisplayName = "OnDestroy"),

};

UCLASS()
class OMEGAGAMEFRAMEWORK_API AOmegaGameplayEffect : public AActor, public IGameplayTagsInterface
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AOmegaGameplayEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Effect Triggered", Category = "Ω|Gameplay|Effects")
		void EffectApplied(float DamageValue);

	UFUNCTION(BlueprintCallable, Category = "Ω|Gameplay|Effects")
		void TriggerEffect();

	UPROPERTY(BlueprintAssignable)
	FOnEffectTriggered OnEffectTriggered;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Ω|Gameplay|Effects")
		void EffectBeginPlay(UObject* Context);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ω|Gameplay|Effects")
		void LifetimeUpdated(float TimeElapsed, float TimeRemaining);

	UPROPERTY(BlueprintReadOnly, Category = "General")
	UCombatantComponent* CombatantInstigator;

	UPROPERTY(BlueprintReadOnly, Category = "General")
	UCombatantComponent* TargetedCombatant;

	UPROPERTY(BlueprintReadOnly, Category = "General")
	UObject* EffectContext;
	
	//-----VOLUME-----//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Volume")
		bool bUseVolume = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Volume")
	EVolumeShape VolumeShape;

	//-----Life-----//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lifetime")
		EEffectLifetime EffectLifetime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lifetime")
		float Lifetime = 1.0;
	
	
	UFUNCTION()
	void LifetimeEnd();

	//-----Damage-----//
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageFormula> DamageFormula = UDamageFormula::StaticClass();

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Damage")
		float Power;

	UFUNCTION(BlueprintPure, Category = "Ω|Gameplay|Effects")
	float CalculateDamageValue();

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		class UDamageFormula* LocalFormula;

	//-----Tags-----//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTag EffectCategory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer EffectTags;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameplayTags")
	FGameplayTag GetObjectGameplayCategory();
	virtual FGameplayTag GetObjectGameplayCategory_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameplayTags")
	FGameplayTagContainer GetObjectGameplayTags();
	virtual FGameplayTagContainer GetObjectGameplayTags_Implementation();

	//###############################################################################################################
	// POPUP
	//###############################################################################################################
	UPROPERTY(EditDefaultsOnly, Category="Popup")
	bool bShowPopupOnTrigger;
	UPROPERTY(EditDefaultsOnly, Category="Popup")
	TSubclassOf<UOmegaEffectPopup> PopupClass;

	TSubclassOf<UOmegaEffectPopup> Local_GetPopupClass()
	{
		if(PopupClass)
		{
			return PopupClass;
		}
		else
		{
			return UOmegaEffectPopup::StaticClass();
		}
	}

	UFUNCTION(BlueprintImplementableEvent, Category="Popup")
	FText GetTriggeredPopupText();
	UFUNCTION(BlueprintImplementableEvent, Category="Popup")
	FSlateColor GetTriggeredPopupColor();

	
	//Misc
	FTimerHandle LifetimeTimer;

	float PastLifetime;
	float RemainingLifetime;
	
};
