// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/GAS.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UGASGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ablility")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
