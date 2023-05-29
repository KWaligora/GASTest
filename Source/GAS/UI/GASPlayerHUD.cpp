// Fill out your copyright notice in the Description page of Project Settings.
#include "GASPlayerHUD.h"

#include "Blueprint/UserWidget.h"

void AGASPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget);
	if (IsValid(PlayerHUD))
	{
		PlayerHUD->AddToViewport();
	}
}
