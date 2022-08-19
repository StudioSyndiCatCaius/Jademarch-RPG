// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowNodes/FlowNode_LinearEvents.h"

#include "Event/OmegaLinearEventInstance.h"



UFlowNode_LinearEvents::UFlowNode_LinearEvents()
{
	InputPins.Empty();
	InputPins.Add(FFlowPin(TEXT("Begin")));
	OutputPins.Empty();
	OutputPins.Add(FFlowPin(TEXT("Finish")));
#if WITH_EDITOR
	Category = TEXT("GameFlow");
#endif
}

void UFlowNode_LinearEvents::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
	LocalInst = GetWorld()->GetSubsystem<UOmegaLinearEventSubsystem>()->PlayLinearEvent(Events, 0);
	LocalInst->OnEventSequenceFinish.AddDynamic(this, &UFlowNode_LinearEvents::LocalFinish);
}

void UFlowNode_LinearEvents::LocalFinish(const FString& Flag)
{
	TriggerFirstOutput(true);
}
