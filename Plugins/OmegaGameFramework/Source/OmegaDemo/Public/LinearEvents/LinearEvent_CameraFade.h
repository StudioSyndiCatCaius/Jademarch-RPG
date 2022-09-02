// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event/OmegaLinearEvent.h"
#include "LinearEvent_CameraFade.generated.h"



UCLASS(DisplayName="Event (Camera Fade)")
class OMEGADEMO_API ULinearEvent_CameraFade : public UOmegaLinearEvent
{
	GENERATED_BODY()

public:
	virtual void Native_Begin() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fade")
	float BeginAlpha = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fade")
	float EndAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fade")
	FLinearColor Color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fade")
	float Duration = 0.5;

	UPROPERTY()
	FTimerHandle Local_TimerHandle;
	UFUNCTION()
	void Local_TimerEnd();
};
