// Retropsis @ 2023-2024

#include "Player/CharacterPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerCharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Input/PlayerInputComponent.h"
#include "UI/Widget/DamageTextComponent.h"

ACharacterPlayerController::ACharacterPlayerController()
{
	bReplicates = true;
}

void ACharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(CharacterContext);

	if(UEnhancedInputLocalPlayerSubsystem*  Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CharacterContext, 0);
	}
}

void ACharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(InputComponent);

	PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::Move);
	PlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::Look);
	PlayerInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::ToggleMenu);
	PlayerInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACharacterPlayerController::BeginInteract);
	PlayerInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACharacterPlayerController::EndInteract);
	PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::Jump);
	PlayerInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::CrouchButtonPressed);
	PlayerInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::EquipButtonPressed);	
	PlayerInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::DropButtonPressed);	
	PlayerInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::AimButtonPressed);	
	PlayerInputComponent->BindAction(TriggerPressedAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::TriggerButtonPressed);	
	PlayerInputComponent->BindAction(TriggerReleasedAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::TriggerButtonReleased);	
	PlayerInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::ThrowButtonPressed);	
	PlayerInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::ReloadButtonPressed);
	
	PlayerInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ACharacterPlayerController::InventoryButtonPressed);
	PlayerInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACharacterPlayerController::Interact_);

	PlayerInputComponent->SetupKeybindInputActions(InputConfig, this, &ThisClass::KeybindInputTagPressed, &ThisClass::KeybindInputTagReleased, &ThisClass::KeybindInputTagHeld);
}

void ACharacterPlayerController::ToggleMenu()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->ToggleMenu();
	}
}


void ACharacterPlayerController::KeybindInputTagPressed(FGameplayTag InputTag)
{
	/*
	 * To Activate some ability if target is undercrosshairs
	 */
	/*if(InputTag.MatchesTagExact(FBaseGameplayTags::Get().SomeAbilityTag))
	{
		bTargeting = ThisActor == nullptr;
		GetHitResultUnderCursor()
	}*/
	// GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(TEXT("%s"), *InputTag.ToString()));
}

void ACharacterPlayerController::KeybindInputTagReleased(FGameplayTag InputTag)
{
	if(GetASC() == nullptr) return;
	GetASC()->KeybindInputTagReleased(InputTag);
}

void ACharacterPlayerController::KeybindInputTagHeld(FGameplayTag InputTag)
{
	if(GetASC() == nullptr) return;
	GetASC()->KeybindInputTagHeld(InputTag);
}

void ACharacterPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(bIsClimbing) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ACharacterPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACharacterPlayerController::BeginInteract()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->BeginInteract();
	}
}

void ACharacterPlayerController::EndInteract()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->EndInteract();
	}
}

void ACharacterPlayerController::Jump()
{
	// TODO: Make it Lazy Init
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->Jump();
	}
}

void ACharacterPlayerController::EquipButtonPressed()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->EquipWeapon();
	}
}

void ACharacterPlayerController::DropButtonPressed()
{
	
}

void ACharacterPlayerController::CrouchButtonPressed()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		if(PlayerCharacter->bIsCrouched)
		{
			PlayerCharacter->UnCrouch();
		}
		else
		{
			PlayerCharacter->Crouch();
		}
	}
}

void ACharacterPlayerController::AimButtonPressed(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->Aim(Value.Get<bool>());
	}
}

void ACharacterPlayerController::TriggerButtonPressed(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->TriggerButtonPressed(Value.Get<bool>());
	}
}

void ACharacterPlayerController::TriggerButtonReleased(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->TriggerButtonReleased(Value.Get<bool>());
	}
}

void ACharacterPlayerController::ReloadButtonPressed(const FInputActionValue& Value)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
		PlayerCharacter->Reload();
	}
}

void ACharacterPlayerController::ThrowButtonPressed(const FInputActionValue& Value)
{
	
}

/*
 * UI
 */
void ACharacterPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void ACharacterPlayerController::SetHUDWeaponAmmo(int32 Ammo) const
{
	OnAmmoAmountChanged.Broadcast(Ammo);
}

void ACharacterPlayerController::SetHUDWeaponCarriedAmmo(int32 Ammo) const
{
	OnCarriedAmmoAmountChanged.Broadcast(Ammo);
}

void ACharacterPlayerController::SetHUDGrenadeCount(int32 GrenadeCount) const
{
	OnGrenadeAmountChanged.Broadcast(GrenadeCount);
}

/*
 * ACCESSORS
 */
UBaseAbilitySystemComponent* ACharacterPlayerController::GetASC()
{
	if(BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BaseAbilitySystemComponent;
}

void ACharacterPlayerController::InventoryButtonPressed()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter()))
	{
	}
}

void ACharacterPlayerController::Interact_()
{
}
