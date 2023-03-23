// Copyright Studio Syndicat 2021. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "OmegaZoneGameInstanceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleteZoneLoadOnLevelLoad);

UCLASS(Category = "OmegaSubsystems|Instance")
class OMEGAGAMEFRAMEWORK_API UOmegaZoneGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Colection) override;
	virtual void Deinitialize() override;

public:
	
	
	void OnLevelChanged(UWorld* World, const UWorld::InitializationValues);
	
	UPROPERTY()
	bool IsInlevelTransit;
	UPROPERTY();
	FGameplayTag TargetSpawnPointTag;
	
};
