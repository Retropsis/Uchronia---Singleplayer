// Retropsis @ 2023-2024

#include "Actor/Weapon/Projectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "UchroniaBlueprintFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Uchronia/Uchronia.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh");
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	// ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}
	if(HasAuthority()) CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) return;

	// TODO: May or Not want to do this
	// if(!UUchroniaBlueprintFunctionLibrary::IsHostile(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	// {
	// 	return;
	// }

	// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 2.f, 12, FColor::Black, false, 3.f);

	// TODO: if Explosion could have a bigger HitReact
	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		CombatInterface->HitReact(Hit.ImpactPoint);
	}
	if(HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
	}
	Destroy();
}

void AProjectile::SpawnTrailSystem()
{
	if(TrailSystem)
	{
		// TODO: Could specify a socket name to be more precise
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,
			GetRootComponent(),
			TrailSocketName,
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false);
	}
}

void AProjectile::ApplyRadialDamageWithFalloff()
{
	// const APawn* FiringPawn = Cast<APawn>(DamageEffectSpecHandle.Data.Get()->GetContext().GetInstigator());
	const APawn* FiringPawn = GetInstigator();
	if(FiringPawn && HasAuthority())
	{
		if(const AController* FiringController = FiringPawn->GetController())
		{
			TArray<AActor*> Overlaps;
			TArray<AActor*> ActorsToIgnore;
			// UGameplayStatics::ApplyRadialDamageWithFalloff() TODO: Function to check for full functionality
			UUchroniaBlueprintFunctionLibrary::GetLivePlayersWithinRadius(
				this,
				Overlaps,
				ActorsToIgnore,
				DamageRadius,
				GetActorLocation()
			);
			// TODO: Apply FallOff, InnerRadius
			if(HasAuthority())
			{
				for(AActor* Overlap : Overlaps)
				{
					if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Overlap))
					{
						TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
					}
				}
			}
		}
	}
}

void AProjectile::DestroyTimerStart()
{
	GetWorldTimerManager().SetTimer(DestroyTimer,this, &AProjectile::DestroyTimerEnd, DestroyTime);
}

void AProjectile::DestroyTimerEnd()
{
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	// TODO: See GAS 122. if this doesn't work completely
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if(IsValid(ImpactSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

