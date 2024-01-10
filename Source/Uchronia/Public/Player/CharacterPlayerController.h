// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoAmountChangedSignature, int32);

class UDamageTextComponent;
class UBaseAbilitySystemComponent;
struct FGameplayTag;
class UInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class UCHRONIA_API ACharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ToggleMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void AimDownSights(bool bUseADS);

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	void SetHUDWeaponAmmo(int32 Ammo) const;
	void SetHUDWeaponCarriedAmmo(int32 Ammo) const;
	void SetHUDGrenadeCount(int32 GrenadeCount) const;
	
	FOnAmmoAmountChangedSignature OnAmmoAmountChanged;
	FOnAmmoAmountChangedSignature OnCarriedAmmoAmountChanged;
	FOnAmmoAmountChangedSignature OnGrenadeAmountChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UBaseAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> CharacterContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>ToggleMenuAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction>InventoryAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> DropAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> TriggerPressedAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> TriggerReleasedAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ThrowAction;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	void KeybindInputTagPressed(FGameplayTag InputTag);
	void KeybindInputTagReleased(FGameplayTag InputTag);
	void KeybindInputTagHeld(FGameplayTag InputTag);
	
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void BeginInteract();
	void EndInteract();
	void Jump();
	void EquipButtonPressed();
	void DropButtonPressed();
	void CrouchButtonPressed();
	void AimButtonPressed(const FInputActionValue& Value);
	void TriggerButtonPressed(const FInputActionValue& Value);
	void TriggerButtonReleased(const FInputActionValue& Value);
	void ReloadButtonPressed(const FInputActionValue& Value);
	void ThrowButtonPressed(const FInputActionValue& Value);

	void InventoryButtonPressed();
	void Interact_();
};


