// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearEvents/LinearEvent_CameraFade.h"

#include "Kismet/GameplayStatics.h"

void ULinearEvent_CameraFade::Native_Begin()
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(BeginAlpha, EndAlpha, Duration, Color, false, true);
	GetWorld()->GetTimerManager().SetTimer(Local_TimerHandle, this, &ULinearEvent_CameraFade::Local_TimerEnd, Duration, false);
	Super::Native_Begin();
}

void ULinearEvent_CameraFade::Local_TimerEnd()
{
	Finish("");
}
