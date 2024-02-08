// Retropsis @ 2023

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Character/BaseCharacter.h"
#include "Interaction/InteractionInterface.h"
#include "Interaction/LootInterface.h"
#include "Types/AnimationStates.h"
#include "Types/CombatState.h"
#include "World/Item.h"
#include "PlayerCharacter.generated.h"

struct FItemStruct;
class APickup;
class UItemBase;
class UInventoryComponent;
class APlayerHUD;
class ARangeWeapon;
class UCombatComponent;
class AWeapon;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};

	UPROPERTY() TObjectPtr<AActor> CurrentInteractable;
	UPROPERTY() float LastInteractionCheckTime;
};

/**
 * 
 */
UCLASS()
class UCHRONIA_API APlayerCharacter : public ABaseCharacter, public ILootInterface/*, public IInteractionInterface*/
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_ReplicatedMovement() override;

	void ToggleMenu();
	void BeginInteract();
	void EndInteract();
	virtual void Jump() override;
	void EquipWeapon();
	void Reload();
	void Aim(bool bIsAiming);
	void TriggerButtonPressed(bool bPressed);
	void TriggerButtonReleased(bool bPressed);

	void UpdateInteractionWidget() const;

	UFUNCTION(Server, Reliable)
	void ServerDropItem(const int32 Quantity);
	
	void DropItem(UItemBase* ItemToDrop, const int32 Quantity);
	
	//~ Combat Interface
	virtual int32 GetCharacterLevel() override;
	virtual void HitReact(const FVector& ImpactPoint) override;
	virtual int32 GetGrenadeCount_Implementation() override;
	virtual void SetGrenadeCount_Implementation(int32 NewCount) override;
	virtual void AdjustGrenadeCount_Implementation(int32 Amount) override;
	virtual void UseAvailableGrenade_Implementation() override;
	//~ Combat Interface

	//~ Loot Interface
	virtual void LootAmmunition(EAmmunitionType AmmunitionType, int32 Amount) override;
	//~ Loot Interface

	virtual void MulticastHandleDeath() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHitReact(const FVector& ImpactPoint);

protected:
	UPROPERTY()
	TObjectPtr<APlayerHUD> PlayerHUD;
	
	// TODO: Could be somewhere else like WidgetController
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere, Category="Character Properties | Inventory")
	TObjectPtr<UInventoryComponent> PlayerInventory;

	/*
	 * Interaction
	 */
	UPROPERTY(ReplicatedUsing=OnRep_TargetInteractable, VisibleAnywhere, Category="Character Properties | Interaction");
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UFUNCTION()
	void OnRep_TargetInteractable();

	float InteractionCheckFrequency = 0.1f;
	float InteractionCheckDistance = 225.f;
	FTimerHandle InteractionCheckTImer;
	FInteractionData InteractionData;

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	
	void Interact();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	// TODO: Could be any pickable actor
	// UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	// TObjectPtr<AWeapon> OverlappingWeapon;

	// UFUNCTION()
	// void OnRep_OverlappingWeapon(AWeapon* LastWeapon) const;

	/*
	 * Inventory
	 */
	UPROPERTY(EditDefaultsOnly, Category="Character Properties | Inventory")
	int32 DefaultInventorySlotsCapacity = 20;
	
	UPROPERTY(EditDefaultsOnly, Category="Character Properties | Inventory")
	float DefaultInventoryWeightCapacity = 50.f;
	
	/*
	 * Combat
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UCombatComponent> CombatComponent;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	float CalculateGroundSpeed();
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	float InterpAO_YawSpeed = 4.f;
	
	/*
	 * Turning In Place
	 */
	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_None;
	void TurnInPlace(float DeltaTime);
	
	void SimProxiesTurn();
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	
	/*
	 * Camera
	 */
	void HideCharacterIfCameraClose();

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	float CameraThreshold = 50.f;

	/*
	 * Ability System
	 */
	virtual void InitAbilityActorInfo() override;
		
public:
	// void SetOverlappingWeapon(AWeapon* Weapon);
	ARangeWeapon* GetEquippedWeapon();
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; };
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return PlayerInventory; };
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
	bool IsMelee() const;

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; };
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; };
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	
	UAnimInstance* GetAnimInstance() const;
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;
	
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(InteractionCheckTImer); };
	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }

	/*
	 * TODO: T3 Should be in a component
	 */
	// void Interact(FVector TraceStart, FVector TraceEnd);
	//
	// UFUNCTION(Server, Reliable, WithValidation)
	// void ServerInteract(FVector TraceStart, FVector TraceEnd);
	
	UPROPERTY(ReplicatedUsing=OnRep_InventoryItems, BlueprintReadWrite)
	TArray<FItemStruct> InventoryItems;

	UFUNCTION()
	void OnRep_InventoryItems();

	void AddInventoryItem(FItemStruct ItemData);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddItemAndUpdateToInventoryWidget(FItemStruct ItemData, const TArray<FItemStruct>& CurrentInventory = TArray<FItemStruct>());

	UFUNCTION(BlueprintCallable)
	void UseItem(TSubclassOf<AItem> ItemSubclass);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem(TSubclassOf<AItem> ItemSubclass);
	
	UPROPERTY(ReplicatedUsing=OnRep_Stats, BlueprintReadWrite)
	float Health = 50.f;
	
	UFUNCTION()
	void OnRep_Stats();
	
	void AddHealth(float Value);
	
	UPROPERTY(ReplicatedUsing=OnRep_Stats, BlueprintReadWrite)
	float Hunger = 100.f;
	
	void RemoveHunger(float Value);

	/*
	 * Shop
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OpenShop(const TArray<FItemStruct>& Items);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryComponent> InventoryComponent;
};