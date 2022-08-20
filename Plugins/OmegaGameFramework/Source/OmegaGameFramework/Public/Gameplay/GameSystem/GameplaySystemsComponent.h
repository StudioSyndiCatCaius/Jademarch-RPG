// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OmegaGameplaySystem.h"
#include "Components/ActorComponent.h"
#include "GameplaySystemsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OMEGAGAMEFRAMEWORK_API UGameplaySystemsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameplaySystemsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Local_ActivateSystems();
	void Local_ShutdownSystems();
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TArray<TSubclassOf<AOmegaGameplaySystem>> GameplaySystems;
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	FString ActivationFlag;

	
};