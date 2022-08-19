// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataInterface_General.h"
#include "Event/OmegaLinearEvent.h"
#include "UObject/Object.h"
#include "LinearEvent_SimpleMessage.generated.h"

// Fires the GlobalEvent "SimpleMessage" with this event as the context. Uses GetGeneralDataTexts to get event text. Finishes on GlobalEvent "EndMessage";
UCLASS(DisplayName="Event (Simple Message)")
class OMEGADEMO_API ULinearEvent_SimpleMessage : public UOmegaLinearEvent, public IDataInterface_General
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, Category="Event")
	FText Title;
	UPROPERTY(EditInstanceOnly, Category="Event", meta=(MultiLine))
	FText Message;

	virtual void GetGeneralDataText_Implementation(const FString& Label, const UObject* Context, FText& Name, FText& Description) override;
	virtual void Native_Begin() override;

	UFUNCTION()
	void LocalGEvent(FName Event, UObject* Context);
};
