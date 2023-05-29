// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GASCharacterAttributeSet.generated.h"

 // Uses macro from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GAS_API UGASCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGASCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, Stamina);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
};
