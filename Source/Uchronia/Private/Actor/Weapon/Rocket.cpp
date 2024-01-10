// Retropsis @ 2023-2024

#include "Actor/Weapon/Rocket.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstanceController.h"
#include "Actor/Weapon/RocketMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ARocket::ARocket()
{
	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>("RocketMovementComponent");
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
	if(!HasAuthority()) CollisionBox->OnComponentHit.AddDynamic(this, &ARocket::OnHit);
	
	SpawnTrailSystem();
	if(ProjectileLoopSound && ProjectileLoopAttenuation)
	{
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(
			ProjectileLoopSound,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			FRotator::ZeroRotator,
			EAttachLocation::KeepWorldPosition,
			false,
			1,1, 0,
			ProjectileLoopAttenuation,
			(USoundConcurrency*) nullptr,
			false);
	}
}

void ARocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                    FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Hit Owner")), true, true, FLinearColor::Blue, 3.f);
		return;
	}
	ApplyRadialDamageWithFalloff();
	DestroyTimerStart();
	
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if(IsValid(ImpactSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	if(ProjectileMesh) ProjectileMesh->SetVisibility(false);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController()) TrailSystemComponent->GetSystemInstanceController()->Deactivate();
	if(ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())  ProjectileLoopComponent->Stop();
}

void ARocket::Destroyed()
{
}