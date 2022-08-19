// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Nodes/World/FlowNode_OnNotifyFromActor.h"
#include "FlowComponent.h"

UFlowNode_OnNotifyFromActor::UFlowNode_OnNotifyFromActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bRetroactive(false)
{
#if WITH_EDITOR
	Category = TEXT("Notifies");
	NodeStyle = EFlowNodeStyle::Condition;
#endif
}

void UFlowNode_OnNotifyFromActor::PostLoad()
{
	Super::PostLoad();

	if (NotifyTag_DEPRECATED.IsValid())
	{
		NotifyTags = FGameplayTagContainer(NotifyTag_DEPRECATED);
	}
}

void UFlowNode_OnNotifyFromActor::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
}

void UFlowNode_OnNotifyFromActor::ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (!RegisteredActors.Contains(Actor))
	{
		RegisteredActors.Emplace(Actor, Component);
		Component->OnNotifyFromComponent.AddUObject(this, &UFlowNode_OnNotifyFromActor::OnNotifyFromComponent);

		if (bRetroactive && Component->GetRecentlySentNotifyTags().HasAnyExact(NotifyTags))
		{
			OnEventReceived();
		}
	}
}

void UFlowNode_OnNotifyFromActor::ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	Component->OnNotifyFromComponent.RemoveAll(this);
}

void UFlowNode_OnNotifyFromActor::OnNotifyFromComponent(UFlowComponent* Component, const FGameplayTag& Tag)
{
	if (Component->IdentityTags.HasAnyExact(IdentityTags) && (!NotifyTags.IsValid() || NotifyTags.HasTagExact(Tag)))
	{
		OnEventReceived();
	}
}

#if WITH_EDITOR
FString UFlowNode_OnNotifyFromActor::GetNodeDescription() const
{
	return GetIdentityTagsDescription(IdentityTags) + LINE_TERMINATOR + GetNotifyTagsDescription(NotifyTags);
}
#endif
