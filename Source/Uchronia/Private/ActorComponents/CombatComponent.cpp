// Retropsis @ 2023-2024

#include "ActorComponents/CombatComponent.h"
#include "Actor/Weapon/Weapon.h"
#include "Character/CharacterAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/CharacterPlayerController.h"
#include "TimerManager.h"
#include "Actor/Weapon/AttachmentComponent.h"
#include "Actor/Weapon/MeleeWeapon.h"
#include "Actor/Weapon/RangeWeapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, EquippedMeleeWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UCombatComponent, CombatState);
	DOREPLIFETIME(UCombatComponent, GrenadeCount);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		if (PlayerCharacter->GetFollowCamera()) 
		{
			DefaultFOV = PlayerCharacter->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
		if(PlayerCharacter->HasAuthority())
		{
			InitializeCarriedAmmo();
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PlayerCharacter && PlayerCharacter->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

/*
 * Combat Functionality
 */
void UCombatComponent::InterpFOV(float DeltaTime)
{
	if(!IsValid(EquippedWeapon)) return;

	if(bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetMarksmanFOV(), DeltaTime, EquippedWeapon->GetMarksmanInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, MarksmanInterpSpeed);		
	}
	if(IsValid(PlayerCharacter) && PlayerCharacter->GetFollowCamera())
	{
		PlayerCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	} 
}

void UCombatComponent::Trigger(const bool bPressed)
{
	bTriggerButtonPressed = bPressed;
	if(bTriggerButtonPressed && IsValid(EquippedWeapon))
	{
		Fire();
	}
}

void UCombatComponent::Fire()
{
	if(CanFire())
	{
		bCanFire = false;	
		ServerTrigger(HitTarget);
        if(IsValid(EquippedWeapon))
        {
        	// TODO: Weapon Recoil Modifier
        	CrosshairRecoilModifier += .2f;
        }
        FireIntervalStart();
	}
}

bool UCombatComponent::CanFire()
{
	if(EquippedWeapon == nullptr) return false;

	if(EquippedWeapon->HasAmmo() && bCanFire && CombatState == ECombatState::ECS_Reloading && EquippedWeapon->CanInterruptReload())
	{
		return true;
	}
	
	if(!EquippedWeapon->HasAmmo())
	{
		if(EquippedWeapon->EmptyContainerSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->EmptyContainerSound, PlayerCharacter->GetActorLocation());
		}
		return false;
	}
	return /*EquippedWeapon->HasAmmo() &&*/ bCanFire && CombatState == ECombatState::ECS_Unoccupied;
}

void UCombatComponent::ServerTrigger_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastTrigger(TraceHitTarget);
}

void UCombatComponent::MulticastTrigger_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(EquippedWeapon == nullptr) return;
	CharacterAnimInstance = CharacterAnimInstance ? CharacterAnimInstance : Cast<UCharacterAnimInstance>(PlayerCharacter->GetAnimInstance());
	if(CharacterAnimInstance && CombatState == ECombatState::ECS_Reloading && EquippedWeapon->CanInterruptReload())
	{
		CharacterAnimInstance->PlayFireMontage(bAiming);
		EquippedWeapon->Trigger(TraceHitTarget);
		CombatState = ECombatState::ECS_Unoccupied;
		return;
	}
	if(CharacterAnimInstance && CombatState == ECombatState::ECS_Unoccupied)
	{
		CharacterAnimInstance->PlayFireMontage(bAiming);
		EquippedWeapon->Trigger(TraceHitTarget);
	}
}

void UCombatComponent::FireIntervalStart()
{
	if(!IsValid(EquippedWeapon) || !IsValid(PlayerCharacter)) return;
	
	PlayerCharacter->GetWorldTimerManager().SetTimer(
		FireIntervalTimer,
		this,
		&UCombatComponent::FireIntervalEnd,
		EquippedWeapon->FireInterval
	);
}

void UCombatComponent::FireIntervalEnd()
{
	if(!IsValid(EquippedWeapon)) return;
	
	bCanFire = true;
	if(bTriggerButtonPressed && EquippedWeapon->bAutomatic)
	{
		Fire();
	}
}

// TODO: Could have its own WidgetController and Widget
void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if(!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->Controller)) return;
	
	CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
	if(IsValid(CharacterPlayerController))
	{
		PlayerHUD = PlayerHUD == nullptr ?  Cast<APlayerHUD>(CharacterPlayerController->GetHUD()) : PlayerHUD;
		if(IsValid(PlayerHUD))
		{
			if(EquippedWeapon)
			{
				HUDPackage.Crosshair_Center = EquippedWeapon->Crosshair_Center;
				HUDPackage.Crosshair_Left = EquippedWeapon->Crosshair_Left;
				HUDPackage.Crosshair_Top = EquippedWeapon->Crosshair_Top;
				HUDPackage.Crosshair_Right = EquippedWeapon->Crosshair_Right;
				HUDPackage.Crosshair_Bottom = EquippedWeapon->Crosshair_Bottom;
			}
			else
			{
				HUDPackage.Crosshair_Center = Crosshair_Center;
				HUDPackage.Crosshair_Left = nullptr;
				HUDPackage.Crosshair_Top = nullptr;
				HUDPackage.Crosshair_Right = nullptr;
				HUDPackage.Crosshair_Bottom = nullptr;
			}
			// Calculate Crosshair Spread
			// [0, 600] -> [0, 1]
			// TODO: APB-like spread
			const FVector2D WalkSpeedRange(0.f, PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed);
			const FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = PlayerCharacter->GetVelocity();
			Velocity.Z = 0.f;

			/* Weapon Modifiers */
			if(EquippedWeapon)
			{
				RunModifier = EquippedWeapon->RunModifier;
				JumpModifier = EquippedWeapon->JumpModifier;
				MarksmanModifier = EquippedWeapon->MarksmanModifier;				
			}
			
			CrosshairVelocityModifier = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if(PlayerCharacter->GetCharacterMovement()->IsFalling())
			{
				// TODO: Weapon Airborne Modifier
				CrosshairAirborneModifier = FMath::FInterpTo(CrosshairAirborneModifier, BaseAirborneSpread, DeltaTime, 2.25f);
			}
			else
			{
				// TODO: Weapon Airborne Recovery Modifier
				CrosshairAirborneModifier = FMath::FInterpTo(CrosshairAirborneModifier, 0.f, DeltaTime, BaseRecoverySpeed);
			}
			if(bAiming)
			{
				CrosshairMarksmanModifier = FMath::FInterpTo(CrosshairMarksmanModifier, 0.58f, DeltaTime, 30.f);
			}
			else
			{
				CrosshairMarksmanModifier = FMath::FInterpTo(CrosshairMarksmanModifier, 0.f, DeltaTime, BaseRecoverySpeed);
			}
			// TODO: Weapon Recoil Recovery
			CrosshairRecoilModifier = FMath::FInterpTo(CrosshairRecoilModifier, 0.f, DeltaTime, 3.f);
			
			// TODO: Try using a min clamp instead
			HUDPackage.CrosshairSpread =
				0.5f +
				CrosshairVelocityModifier * RunModifier +
				CrosshairAirborneModifier * JumpModifier -
				CrosshairMarksmanModifier * MarksmanModifier +
				CrosshairRecoilModifier;
			PlayerHUD->SetHUDPackage(HUDPackage);
		}
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	// TODO: Could add a parameterizable offset x and y
	const FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if(IsValid(PlayerCharacter))
		{
			const float DistanceToCharacter = (PlayerCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + TraceExtent);
			// UKismetSystemLibrary::DrawDebugSphere(this, Start, 12.f, 12, FLinearColor::Blue);
		}
		
		const FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
		);
		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UCrosshairInterface>())
		{
			HUDPackage.CrosshairColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairColor = FLinearColor::White;			
		}
	}
}

void UCombatComponent::SetAiming(const bool IsAiming)
{
	if(!IsValid(PlayerCharacter) || !IsValid(EquippedWeapon)) return;
	
	bAiming = IsAiming;
	ServerSetAiming(IsAiming);
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
	// TODO: Need to check from DataAsset Info instead
	if(PlayerCharacter->IsLocallyControlled() && EquippedWeapon->CanAimDownSights())
	{
		CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
		if(CharacterPlayerController)
		{
				CharacterPlayerController->AimDownSights(bAiming);
		}
	}
}

void UCombatComponent::ServerSetAiming_Implementation(const bool IsAiming)
{
	bAiming = IsAiming;
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

/*
 * Equipping START
 */
void UCombatComponent::EquipWeapon(AWeapon*  WeaponToEquip)
{
	if(PlayerCharacter == nullptr || WeaponToEquip == nullptr) return;
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	
	DropEquippedWeapon();
	if(ARangeWeapon* RangedWeaponToEquip = Cast<ARangeWeapon>(WeaponToEquip))
	{
		EquippedWeapon = RangedWeaponToEquip;
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachActorToSocket(EquippedWeapon, FName("RightHandSocket"));
		EquippedWeapon->SetOwner(PlayerCharacter); // is replicated
		EquippedWeapon->SetHUDAmmo();
		UpdateCarriedAmmo();
		PlayEquipSound();
	}
	if(AMeleeWeapon* MeleeWeaponToEquip = Cast<AMeleeWeapon>(WeaponToEquip))
	{
		EquippedMeleeWeapon = MeleeWeaponToEquip;
		EquippedMeleeWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachActorToSocket(EquippedMeleeWeapon, FName("RightHandSocket"));
		EquippedMeleeWeapon->SetOwner(PlayerCharacter); // is replicated
		if(EquippedMeleeWeapon && EquippedMeleeWeapon->EquipSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, EquippedMeleeWeapon->EquipSound, PlayerCharacter->GetActorLocation());
		}
	}
	
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	PlayerCharacter->bUseControllerRotationYaw = true;
}

void UCombatComponent::InitializeWeapon(AWeapon* WeaponToInitialize)
{
}

AWeapon* UCombatComponent::SetupAttachments(TSubclassOf<AWeapon> WeaponToSetup, TMap<EAttachmentType, FAttachmentData> Attachments)
{
	if(IsValid(WeaponToSetup))
	{
		FActorSpawnParameters SpawnParams;
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(
			WeaponToSetup,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		if(Weapon && Weapon->GetWeaponMesh())
		{
			for (TTuple<EAttachmentType, FAttachmentData> Attachment : Attachments)
			{
				FTransform SocketTransform = Weapon->GetWeaponMesh()->GetSocketTransform(Attachment.Value.AttachmentSocketName);
				if (UAttachmentComponent* AttachmentComponent = Cast<UAttachmentComponent>(Weapon->AddComponentByClass(Attachment.Value.AttachmentComponentClass, false, SocketTransform, false)))
				{
					AttachmentComponent->AttachmentType = Attachment.Key;
					AttachmentComponent->InitializeAttachment(Attachment.Value.AttachmentInitializationData);
                    Weapon->AttachmentMap.Add(Attachment.Key, AttachmentComponent);
				}
			}
		}
		return Weapon;
	}
	return nullptr;
}

FAttachmentData UCombatComponent::UpdateAttachment(const EAttachmentType AttachmentType, FAttachmentData NewAttachment)
{
	FAttachmentData RemovedAttachment = FAttachmentData();
	if(IsValid(EquippedWeapon))
	{
		// Remove first old attachment if any
		TArray<UAttachmentComponent*> EquippedWeaponAttachments;
		EquippedWeapon->GetComponents(UAttachmentComponent::StaticClass(), EquippedWeaponAttachments);
		for (UAttachmentComponent* Attachment : EquippedWeaponAttachments)
		{
			if (Attachment->AttachmentType == AttachmentType)
			{
				RemovedAttachment.AttachmentComponentClass = Attachment->StaticClass();
				RemovedAttachment.AttachmentType = Attachment->AttachmentType;
				Attachment->DestroyComponent(false);
			}
		}
		// Add the new attachment
		if(IsValid(NewAttachment.AttachmentComponentClass))
		{
			const FTransform SocketTransform = UKismetMathLibrary::InvertTransform(EquippedWeapon->GetWeaponMesh()->GetSocketTransform(NewAttachment.AttachmentSocketName));
			if (UAttachmentComponent* AttachmentComponent = Cast<UAttachmentComponent>(EquippedWeapon->AddComponentByClass(NewAttachment.AttachmentComponentClass, false, FTransform(), false)))
			{
				AttachmentComponent->AttachmentType = AttachmentType;
				AttachmentComponent->InitializeAttachment(NewAttachment.AttachmentInitializationData);
				EquippedWeapon->AttachmentMap.Add(AttachmentType, AttachmentComponent);
			}
		}
	}
	return RemovedAttachment;
}

void UCombatComponent::UpdateAttachments(TMap<EAttachmentType, FAttachmentData> Attachments)
{
	if(IsValid(EquippedWeapon))
	{
			TArray<UAttachmentComponent*> EquippedWeaponAttachments;
        	EquippedWeapon->GetComponents(UAttachmentComponent::StaticClass(), EquippedWeaponAttachments);
        	for (UAttachmentComponent* Attachment : EquippedWeaponAttachments)
        	{
        		GEngine->AddOnScreenDebugMessage(123, 15.f, FColor::Magenta, FString::Printf(TEXT("Attachment: %s"), *Attachment->GetName()));
        	}
	}
}

void UCombatComponent::UnequipWeapon(AWeapon* WeaponToUnequip)
{
	if(EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}
	if(EquippedMeleeWeapon)
	{
		EquippedMeleeWeapon->Destroy();
		EquippedMeleeWeapon = nullptr;
	}
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	PlayerCharacter->bUseControllerRotationYaw = false;
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if(IsValid(EquippedWeapon) && IsValid(PlayerCharacter))
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachActorToSocket(EquippedWeapon, FName("RightHandSocket"));
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
		EquippedWeapon->SetHUDAmmo();
		PlayEquipSound();
	}
}

void UCombatComponent::OnRep_EquippedMeleeWeapon()
{
	if(IsValid(EquippedMeleeWeapon) && IsValid(PlayerCharacter))
	{
		EquippedMeleeWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		AttachActorToSocket(EquippedMeleeWeapon, FName("RightHandSocket"));
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
		PlayEquipSound();
	}
}

void UCombatComponent::DropEquippedWeapon() const
{
	// TODO: Find how to properly reset these pointers or find another solution
	if(EquippedWeapon)
	{
		EquippedWeapon->Drop();
		EquippedWeapon->MarkAsGarbage();
	}
	if(EquippedMeleeWeapon)
	{
		EquippedMeleeWeapon->Drop();
		EquippedMeleeWeapon->MarkAsGarbage();
	}
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	PlayerCharacter->bUseControllerRotationYaw = false;
}

void UCombatComponent::UpdateCarriedAmmo()
{
	if(!IsValid(EquippedWeapon)) return;
	
	if(CarriedAmmoMap.Contains(EquippedWeapon->GetAmmunitionType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetAmmunitionType()];
	}
	CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
	if(CharacterPlayerController)
	{
		CharacterPlayerController->SetHUDWeaponCarriedAmmo(CarriedAmmo);
	}
}

void UCombatComponent::PlayEquipSound() const
{
	if(EquippedWeapon && EquippedWeapon->EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->EquipSound, PlayerCharacter->GetActorLocation());
	}
}
/*
 * Equipping END
 */

/*
 * Ammo START
 */
void UCombatComponent::OnRep_CarriedAmmo()
{
	CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
	if(CharacterPlayerController)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Carried Ammo: [%d]"), CarriedAmmo), true, true, FLinearColor::Blue, 3.f);
		CharacterPlayerController->SetHUDWeaponCarriedAmmo(CarriedAmmo);
	}
	bool bJumpToReloadEnd = CombatState == ECombatState::ECS_Reloading &&
		IsValid(EquippedWeapon) && EquippedWeapon->CanInterruptReload() &&
			CarriedAmmo == 0;
	if(bJumpToReloadEnd)
	{
		JumpToReloadEnd();
	}
}

void UCombatComponent::Reload()
{
	// if(EquippedWeapon == nullptr) return;
	//TODO: Design choice if we can reload even full
	if(CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied /*&& EquippedWeapon && !EquippedWeapon->IsFull()*/)
	{
		ServerReload();
	}
}

void UCombatComponent::ServerReload_Implementation()
{
	CombatState = ECombatState::ECS_Reloading;
	HandleReload();
}

void UCombatComponent::HandleReload()
{
	CharacterAnimInstance = CharacterAnimInstance ? CharacterAnimInstance : Cast<UCharacterAnimInstance>(PlayerCharacter->GetAnimInstance());
	if(CharacterAnimInstance)
	{
		CharacterAnimInstance->PlayReloadMontage();
	}
}

void UCombatComponent::AddSingleRound()
{
	if(PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		UpdateSingleAmmoValue();
	}
}

void UCombatComponent::ReloadEnd()
{
	if(PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}
	if (bTriggerButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::UpdateAmmoValues()
{
	if(!IsValid(EquippedWeapon)) return;
	
	int32 ReloadAmount = AmountToReload();
	if(CarriedAmmoMap.Contains(EquippedWeapon->GetAmmunitionType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetAmmunitionType()] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetAmmunitionType()];
	}
	// Updating Weapon Type Carried Ammo to HUD
	CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
	if(CharacterPlayerController)
	{
		CharacterPlayerController->SetHUDWeaponCarriedAmmo(CarriedAmmo);
	}
	EquippedWeapon->AddRounds(ReloadAmount);
}

// TODO: Refacto this and UpdateAmmoValues with params
void UCombatComponent::UpdateSingleAmmoValue()
{
	if(!IsValid(EquippedWeapon)) return;
	
	if(CarriedAmmoMap.Contains(EquippedWeapon->GetAmmunitionType()))
	{
		CarriedAmmoMap[EquippedWeapon->GetAmmunitionType()] -= 1;
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetAmmunitionType()];
	}
	// Updating Weapon Type Carried Ammo to HUD
	CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
	if(CharacterPlayerController)
	{
		CharacterPlayerController->SetHUDWeaponCarriedAmmo(CarriedAmmo);
	}
	EquippedWeapon->AddRounds(1);
	bCanFire = true;
	if(EquippedWeapon->IsFull() || CarriedAmmo == 0)
	{
		JumpToReloadEnd();
	}
}

void UCombatComponent::JumpToReloadEnd()
{
	CharacterAnimInstance = CharacterAnimInstance ? CharacterAnimInstance : Cast<UCharacterAnimInstance>(PlayerCharacter->GetAnimInstance());
	if(CharacterAnimInstance)
	{
		CharacterAnimInstance->JumpToReloadEnd();
	}
}

int32 UCombatComponent::AmountToReload()
{
	if(EquippedWeapon == nullptr) return 0;
	const int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();
	
	if(CarriedAmmoMap.Contains(EquippedWeapon->GetAmmunitionType()))
	{
		const int32 AmountCarried = CarriedAmmoMap[EquippedWeapon->GetAmmunitionType()];
		const int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least);
	}
	return 0;
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState) {
	case ECombatState::ECS_Unoccupied:
		if(bTriggerButtonPressed)
		{
			Fire();
		}
		break;
	case ECombatState::ECS_Reloading:
		HandleReload();
		break;
	case ECombatState::ECS_Throwing:
		AttachActorToSocket(EquippedWeapon, FName("LeftHandSocket"));
		ShowThrowableItem(true);
		break;
	default: ;
	}
}

void UCombatComponent::Throw()
{
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	CombatState = ECombatState::ECS_Throwing;
	ShowThrowableItem(true);
}

void UCombatComponent::ServerThrow_Implementation()
{
	CombatState = ECombatState::ECS_Throwing;
	// TODO: Could be AbilityTask responsibility ?
	AttachActorToSocket(EquippedWeapon, FName("LeftHandSocket"));
	ShowThrowableItem(true);
}

/*
 * Anim Notifies
 */
void UCombatComponent::ThrowStart()
{
	CombatState = ECombatState::ECS_Throwing;
	// TODO: Could be AbilityTask responsibility ?
	AttachActorToSocket(EquippedWeapon, FName("LeftHandSocket"));
	ShowThrowableItem(true);
	ServerThrow();
}

void UCombatComponent::ThrowItem()
{
	ShowThrowableItem(false);
}

void UCombatComponent::ThrowEnd()
{
	CombatState = ECombatState::ECS_Unoccupied;
	AttachActorToSocket(EquippedWeapon, FName("RightHandSocket"));
}

void UCombatComponent::MeleeStart()
{
	CombatState = ECombatState::ECS_CastingAbility;
}

void UCombatComponent::MeleeEnd()
{
	CombatState = ECombatState::ECS_Unoccupied;
}

void UCombatComponent::SetMeleeWeaponCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled)
{
	if(PlayerCharacter->HasAuthority() && EquippedMeleeWeapon && EquippedMeleeWeapon->GetCollisionBox())
	{
		EquippedMeleeWeapon->GetCollisionBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedMeleeWeapon->EmptyIgnoreActors();
	}
}
/*
 * Anim Notifies - END
 */

void UCombatComponent::UpdateHUDGrenades()
{
	CharacterPlayerController = CharacterPlayerController == nullptr ?  Cast<ACharacterPlayerController>(PlayerCharacter->Controller) : CharacterPlayerController;
	if(CharacterPlayerController)
	{
		CharacterPlayerController->SetHUDGrenadeCount(GrenadeCount);
	}
}

void UCombatComponent::OnRep_GrenadeCount()
{
	UpdateHUDGrenades();
}

/*
 * Utility
 */
// TODO: Could be some library function specific to PlayerCharacter
void UCombatComponent::AttachActorToSocket(AActor* ActorToAttach, const FName Socket) const
{
	if(!IsValid(PlayerCharacter) || PlayerCharacter->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	
	if(const USkeletalMeshSocket* SocketToAttach= PlayerCharacter->GetMesh()->GetSocketByName(Socket))
	{
		SocketToAttach->AttachActor(ActorToAttach, PlayerCharacter->GetMesh());
	}
}

void UCombatComponent::ShowThrowableItem(const bool bShowItem) const
{
	if(IsValid(PlayerCharacter) && PlayerCharacter->GetThrowableItem())
	{
		PlayerCharacter->GetThrowableItem()->SetVisibility(bShowItem);
	}
}

void UCombatComponent::PickupAmmunition(const EAmmunitionType AmmunitionType, int32 Amount)
{
	if(CarriedAmmoMap.Contains(AmmunitionType))
	{
		CarriedAmmoMap[AmmunitionType] = FMath::Clamp(CarriedAmmoMap[AmmunitionType] + Amount, 0, MaxCarriedAmmo);
		UpdateCarriedAmmo();
	}
}

/*
 * For DEV ONLY
 */
void UCombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_9x19mm, Starting9mmAmmo);
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_M6A1, StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_Energy, StartingLaserAmmo);
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_45, StartingSubmachineGunAmmo);
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_12ga, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_308, StartingHighCaliberRifleAmmo);
	CarriedAmmoMap.Emplace(EAmmunitionType::EAT_40mm, StartingGrenadeLauncherAmmo);
}