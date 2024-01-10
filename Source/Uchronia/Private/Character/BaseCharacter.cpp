// Retropsis @ 2023

#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("LeftHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// TODO: Investigate
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	ThrowableItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowableItem"));
	// ThrownItem->SetupAttachment(GetMesh(), ThrownItemSocketName);
	ThrowableItem->SetupAttachment(GetMesh(), FName("ThrownItemSocket"));
	ThrowableItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(ThrowableItem))
	{
		ThrowableItem->SetVisibility(false);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	UBaseAbilitySystemComponent* BaseASC = CastChecked<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	BaseASC->AddCharacterAbilities(StartupAbilities);
}

/* ASC Interface */
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
/* ASC Interface */

/* Combat Interface */
FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	// TODO: Make it data driven, TMap
	// TODO: Expand to other sockets
	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(CombatSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Hand_Left))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Hand_Right))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

void ABaseCharacter::HitReact(const FVector& ImpactPoint)
{
}

void ABaseCharacter::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}


void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	// TODO: do this to the EquippedWeapon instead
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	if(DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	bDead = true;
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

/* Combat Interface */
UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	// TODO: Move this to AnimInstance
	return HitReactMontage;
	// if(const UAIAnimInstance* AIAnimInstance = Cast<UAIAnimInstance>(GetMesh()->GetAnimInstance())) return AIAnimInstance->GetHitReactMontage();
	// return nullptr;
}

/*
 * Swimming
 */
void ABaseCharacter::SetSwimmingState_Implementation(bool bShouldSwim)
{
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = bShouldSwim;
	GetCharacterMovement()->SetMovementMode(bSwimming ? MOVE_Swimming : MOVE_Walking);
	bSwimming = bShouldSwim;
}

/*
 * Dissolve
 */
void ABaseCharacter::Dissolve()
{
	if(IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ABaseCharacter::GetSoftBodyImpact_Implementation()
{
	return SoftBodyImpact;
}

USoundBase* ABaseCharacter::GetSoftBodySound_Implementation()
{
	return SoftBodySound;
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for(FTaggedMontage TaggedMontage : AttackMontages)
	{
		if(TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ABaseCharacter::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}
