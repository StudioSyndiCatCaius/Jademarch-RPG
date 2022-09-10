// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Combatant/OmegaEffectPopup.h"

#include "OmegaAttribute.h"
#include "OmegaGameplayEffect.h"
#include "Gameplay/CombatantComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UOmegaEffectPopup::NativeConstruct()
{
	Super::NativeConstruct();

	if(GetDamageText())
	{
		GetDamageText()->SetText(OwningEffect->GetTriggeredPopupText());
		GetDamageText()->SetColorAndOpacity(OwningEffect->GetTriggeredPopupColor());
	}
	
	GetWorld()->GetTimerManager().SetTimer(LifetimeTimer, this, &UOmegaEffectPopup::RemoveFromParent, GetPopupLifetime(), false);
}

void UOmegaEffectPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentPosition = UKismetMathLibrary::Vector2DInterpTo(CurrentPosition, InitPosition+GetTargetViewportPosition(), InDeltaTime, 1/GetPopupLifetime());
	SetPositionInViewport(CurrentPosition);
	
}


float UOmegaEffectPopup::GetPopupLifetime_Implementation()
{
	return 2.0;
}
