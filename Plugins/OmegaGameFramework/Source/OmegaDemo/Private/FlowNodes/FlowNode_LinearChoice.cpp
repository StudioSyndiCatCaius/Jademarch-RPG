// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowNodes/FlowNode_LinearChoice.h"

#include "OmegaLinearEventSubsystem.h"

UFlowNode_LinearChoice::UFlowNode_LinearChoice()
{
	InputPins.Empty();
	InputPins.Add(FFlowPin(TEXT("Begin")));
	OutputPins.Empty();
	NodeStyle = EFlowNodeStyle::Latent;
#if WITH_EDITOR
	Category = TEXT("GameFlow");
#endif
}

void UFlowNode_LinearChoice::LocalChoiceSelect(UOmegaLinearChoice* Choice, int32 Index)
{
	FString OutputLocalName = FString::FromInt(Index);
	TriggerOutput(FName(OutputLocalName));
}

bool UFlowNode_LinearChoice::CanUserAddOutput() const
{
	return true;
}

void UFlowNode_LinearChoice::ExecuteInput(const FName& PinName)
{
	ChoiceInst = GetWorld()->GetSubsystem<UOmegaLinearEventSubsystem>()->PlayLinearChoice(Choices, InstanceClass);
	ChoiceInst->OnChoiceSelected.AddDynamic(this, &UFlowNode_LinearChoice::LocalChoiceSelect);
	
	Super::ExecuteInput(PinName);
}
