// Retropsis @ 2023-2024

#include "Character/CharacterAnimInstance.h"
#include "Actor/Weapon/RangeWeapon.h"
#include "Actor/Weapon/Weapon.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TODO: check lazy init
	if(PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	if(PlayerCharacter == nullptr) return;

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	GroundSpeed = Velocity.Size();

	bAirborne = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
	bSwimming = PlayerCharacter->IsSwimming();
	bWeaponEquipped = PlayerCharacter->IsWeaponEquipped();
	EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	
	bIsCrouched = PlayerCharacter->bIsCrouched;
	bAiming = PlayerCharacter->IsAiming();
	TurningInPlace = PlayerCharacter->GetTurningInPlace();
	bRotateRootBone = PlayerCharacter->ShouldRotateRootBone();

	// Offset Yaw for Strafing
	const FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
	const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, YawOffsetInterpSpeed);
	YawOffset = DeltaRotation.Yaw;

	// Leaning
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PlayerCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, LeanInterpSpeed);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = PlayerCharacter->GetAO_Yaw();
	AO_Pitch = PlayerCharacter->GetAO_Pitch();

	if(bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && PlayerCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		const FRotator InRotation = LeftHandTransform.GetRotation().Rotator();
		PlayerCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), InRotation, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		// TODO: Need HitTarget to replicate (NetQuantize?) for simulated proxies
		if(PlayerCharacter->IsLocallyControlled())
		{
			bIsLocallyControlled = true;
			const FTransform RightHandTransform = PlayerCharacter->GetMesh()->GetSocketTransform(FName("hand_r"), RTS_World);

			// RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - PlayerCharacter->GetHitTarget()));
			// RightHandRotation = UKismetMathLibrary::FindLookAtRotation(FVector(), (RightHandTransform.GetLocation() - PlayerCharacter->GetHitTarget()));
			
			// FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + PlayerCharacter->GetHitTarget());
			// FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(FVector(), RightHandTransform.GetLocation() - PlayerCharacter->GetHitTarget());
			
			// FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - PlayerCharacter->GetHitTarget()));
			
			// LookAtRotation.Roll += EquippedWeapon->RightHandRotationRoll; 
			// LookAtRotation.Yaw += EquippedWeapon->RightHandRotationYaw; 
			// LookAtRotation.Pitch += EquippedWeapon->RightHandRotationPitch;
			
			// RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaSeconds, 30.f);

			RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - PlayerCharacter->GetHitTarget()));
			RightHandRotation.Yaw -= 90.f;
		
			const FTransform MuzzleFlashTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), RTS_World);
			const FVector MuzzleX(FRotationMatrix(MuzzleFlashTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			UKismetSystemLibrary::DrawDebugLine(this, MuzzleFlashTransform.GetLocation(), MuzzleFlashTransform.GetLocation() + MuzzleX * 1000.f, FLinearColor::Red);
			UKismetSystemLibrary::DrawDebugLine(this, MuzzleFlashTransform.GetLocation(), PlayerCharacter->GetHitTarget(), FLinearColor::Blue);
		}
	}
	bUseFABRIK = PlayerCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !PlayerCharacter->IsMelee();
	bUseAimOffsets = PlayerCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !PlayerCharacter->IsMelee();
	bTransformRightHand = PlayerCharacter->GetCombatState() == ECombatState::ECS_Unoccupied && !PlayerCharacter->IsMelee();
}

void UCharacterAnimInstance::PlayFireMontage(bool bIsAiming)
{
	if(PlayerCharacter->GetCombatComponent() == nullptr || PlayerCharacter->GetEquippedWeapon() == nullptr) return;
	if(IsValid(FireWeaponMontage))
	{
		Montage_Play(FireWeaponMontage);
		const FName SectionName = bIsAiming ? FName("TwoHands_Aim") : FName("TwoHands_Hip");
		Montage_JumpToSection(SectionName);
	}
}

void UCharacterAnimInstance::PlayReloadMontage()
{
	if(PlayerCharacter->GetCombatComponent() == nullptr || PlayerCharacter->GetEquippedWeapon() == nullptr) return;
	if(IsValid(ReloadMontage))
	{
		Montage_Play(ReloadMontage);
		Montage_JumpToSection(EquippedWeapon->GetReloadSectionName());
	}
}

// TODO: Probably need to adapt for different weapons if needed
void UCharacterAnimInstance::JumpToReloadEnd()
{
	if(IsValid(ReloadMontage))
	{
		Montage_JumpToSection(FName("ReloadEnd"));
	}
}

void UCharacterAnimInstance::PlayHitReactMontage(const FVector& ImpactPoint)
{
	if(PlayerCharacter->GetCombatComponent() == nullptr || PlayerCharacter->GetEquippedWeapon() == nullptr) return;
	
	if(IsValid(HitReactMontage))
	{
		DirectionalHitReact(ImpactPoint);
	}
}

void UCharacterAnimInstance::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = PlayerCharacter->GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, PlayerCharacter->GetActorLocation().Z);
	const FVector ToHit = ( ImpactLowered - PlayerCharacter->GetActorLocation() ).GetSafeNormal();

	// Forward .ToHit = |Forward| |ToHit| * cos(theta)
	//  |Forward| = 1,  |ToHit| = 1, so Forward .ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
		
	// if CrossProduct points down, theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if(CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	// GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Emerald, FString::Printf(TEXT("Theta: [%f]"), Theta));
	// UKismetSystemLibrary::DrawDebugArrow(this, PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation( ) + Forward * 50.f, 1.f, FLinearColor::Red, 3.f, 1.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation( ) + ToHit * 50.f, 1.f, FLinearColor::Green, 3.f, 1.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation( ) + CrossProduct * 50.f, 1.f, FLinearColor::Blue, 3.f, 1.f);
		
	Montage_Play(HitReactMontage);
	FName Section("FromBack");

	if(Theta >= -45.f && Theta <= 45.f) Section = FName("FromFront");
	else if (Theta >= -135.f && Theta < -45.f) Section = FName("FromLeft");
	else if (Theta >= 45.f && Theta < 135.f) Section = FName("FromRight");
		
	Montage_JumpToSection(Section);
}

EPhysicalSurface UCharacterAnimInstance::GetSurfaceType()
{
	FHitResult HitResult;
	const FVector Start{GetOwningActor()->GetActorLocation()};
	const FVector End{Start + FVector(0.f, 0.f, -400.f)};
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

	return UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
}
