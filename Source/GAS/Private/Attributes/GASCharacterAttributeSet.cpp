// Fill out your copyright notice in the Description page of Project Settings.


#include "Attributes/GASCharacterAttributeSet.h"

#include "Net/UnrealNetwork.h"

UGASCharacterAttributeSet::UGASCharacterAttributeSet()
{
	
}

void UGASCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGASCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
}

void UGASCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCharacterAttributeSet, Health, OldHealth);
}

void UGASCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCharacterAttributeSet, Stamina, OldStamina);
}
