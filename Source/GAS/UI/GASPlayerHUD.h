// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GASPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AGASPlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> PlayerHUDWidget;

	UPROPERTY(Transient)
	UUserWidget* PlayerHUD;

protected:
	virtual void BeginPlay() override;
};
