// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GASAbilitySystemComponent.h"
#include "Abilities/GASGameplayAbility.h"
#include "Attributes/GASCharacterAttributeSet.h"


//////////////////////////////////////////////////////////////////////////
// AGASCharacter

AGASCharacter::AGASCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GASAbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	GASAbilitySystemComponent->SetIsReplicated(true);
	GASAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	GASAttributeSet = CreateDefaultSubobject<UGASCharacterAttributeSet>(TEXT("Attributes"));
}

void AGASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AGASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASCharacter::Look);
	}

	if (IsValid(GASAbilitySystemComponent) && IsValid(InputComponent))
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		GASAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}


void AGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGASCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGASCharacter::GetHasRifle()
{
	return bHasRifle;
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComponent;
}

void AGASCharacter::InitializeAttributes()
{
	if (IsValid(GASAbilitySystemComponent) && IsValid(DefaultAttributeEffect))
	{
		FGameplayEffectContextHandle EffectContextHandle = GASAbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = GASAbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContextHandle);

		if (SpecHandle.IsValid())
		{
			GASAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AGASCharacter::GiveStartingAbilities()
{
	if (HasAuthority() && IsValid(GASAbilitySystemComponent))
	{
		for (TSubclassOf<UGASGameplayAbility> GameplayAbility : DefaultAbilities)
		{
			GASAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GameplayAbility, 1, static_cast<int32>(GameplayAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(GASAbilitySystemComponent))
	{
		//Init on Server
		GASAbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		GiveStartingAbilities();
	}	
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsValid(GASAbilitySystemComponent))
	{
		//Init on Client
		GASAbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();

		if (IsValid(InputComponent))
		{
			const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
			GASAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
		}
	}		
}
