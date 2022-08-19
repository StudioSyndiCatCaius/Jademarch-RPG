// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowNodes/FlowNode_SaveCondition.h"

UFlowNode_SaveCondition::UFlowNode_SaveCondition()
{
	InputPins.Empty();
	InputPins.Add(FFlowPin(TEXT("In")));
	OutputPins.Empty();
	OutputPins.Add(FFlowPin(TEXT("True")));
	OutputPins.Add(FFlowPin(TEXT("False")));
	
	NodeStyle = EFlowNodeStyle::Condition;
#if WITH_EDITOR
	Category = TEXT("GameFlow");
#endif
}

void UFlowNode_SaveCondition::ExecuteInput(const FName& PinName)
{
	bool LocalSuccess = GetWorld()->GetGameInstance()->GetSubsystem<UOmegaSaveSubsystem>()->CustomSaveConditionsMet(Conditions);
	if(LocalSuccess)
	{
		TriggerOutput(FName("True"));
	}
	else
	{
		TriggerOutput(FName("False"));
	}
}
