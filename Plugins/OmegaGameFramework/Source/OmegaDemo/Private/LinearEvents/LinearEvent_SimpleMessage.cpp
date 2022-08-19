// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearEvents/LinearEvent_SimpleMessage.h"

#include "OmegaGameManager.h"

void ULinearEvent_SimpleMessage::GetGeneralDataText_Implementation(const FString& Label, const UObject* Context,
                                                                   FText& Name, FText& Description)
{
	Name = Title;
	Description = Message;
}

void ULinearEvent_SimpleMessage::Native_Begin()
{
	Super::Native_Begin();
	
	GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>()->FireGlobalEvent(FName("SimpleMessage"), this);
	GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>()->OnGlobalEvent.AddDynamic(this, &ULinearEvent_SimpleMessage::LocalGEvent);
	
}

void ULinearEvent_SimpleMessage::LocalGEvent(FName Event, UObject* Context)
{
	if(Event==FName("EndMessage"))
	{
		GetWorld()->GetGameInstance()->GetSubsystem<UOmegaGameManager>()->OnGlobalEvent.RemoveDynamic(this, &ULinearEvent_SimpleMessage::LocalGEvent);
		Finish("");
	}
	
}
