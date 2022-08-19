// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OmegaAbility.h"
#include "Components/TimelineComponent.h"
#include "Ability_Timeline.generated.h"

UCLASS(Abstract)
class OMEGADEMO_API AAbility_Timeline : public AOmegaAbility
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbility_Timeline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Native_AbilityActivated(UObject* Context) override;
	virtual void Native_AbilityFinished(bool Cancelled) override;
	virtual bool Native_CanActivate(UObject* Context) override;
	
	UFUNCTION(BlueprintNativeEvent, Category="Timeline Ability")
	UTimelineComponent* GetRootAbilityTimeline();
};
