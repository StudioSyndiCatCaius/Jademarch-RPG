// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/Operators/FlowNode_LogicalAND.h"

UFlowNode_LogicalAND::UFlowNode_LogicalAND(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Operators");
	NodeStyle = EFlowNodeStyle::Logic;
#endif

	SetNumberedInputPins(0, 1);
}

void UFlowNode_LogicalAND::ExecuteInput(const FName& PinName)
{
	ExecutedInputNames.Add(PinName);

	if (ExecutedInputNames.Num() == InputPins.Num())
	{
		TriggerFirstOutput(true);
	}
}

void UFlowNode_LogicalAND::Cleanup()
{
	ExecutedInputNames.Empty();
}
