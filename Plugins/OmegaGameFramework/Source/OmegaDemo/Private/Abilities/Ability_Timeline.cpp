// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Ability_Timeline.h"


// Sets default values
AAbility_Timeline::AAbility_Timeline()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAbility_Timeline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbility_Timeline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbility_Timeline::Native_AbilityActivated(UObject* Context)
{
	GetRootAbilityTimeline()->PlayFromStart();
	Super::Native_AbilityActivated(Context);
}

void AAbility_Timeline::Native_AbilityFinished(bool Cancelled)
{
	GetRootAbilityTimeline()->Stop();
	GetRootAbilityTimeline()->SetPlaybackPosition(GetRootAbilityTimeline()->GetTimelineLength(),false, false);
	Super::Native_AbilityFinished(Cancelled);
}

bool AAbility_Timeline::Native_CanActivate(UObject* Context)
{
	return Super::Native_CanActivate(Context) && GetRootAbilityTimeline()->IsValidLowLevel();
}

UTimelineComponent* AAbility_Timeline::GetRootAbilityTimeline_Implementation()
{
	return nullptr;
}

