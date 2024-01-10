// Retropsis @ 2023-2024


#include "Actor/Weapon/MeleeWeapon.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AMeleeWeapon::AMeleeWeapon()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
		CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBoxBeginOverlap);
	}
}

void AMeleeWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for(AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this, Start, End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::White,
		5.f
	);
	AActor* HitActor = HitResult.GetActor();
	if(!bIsMultiHit && HitActor)
	{
		IgnoreActors.AddUnique(HitActor);
	}
	CauseDamage(HitResult);
	if(HitActor)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(HitActor);
		if(USoundBase* ImpactSound = CombatInterface->Execute_GetSoftBodySound(HitActor))
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		}
		if(UNiagaraSystem* ImpactEffect = CombatInterface->Execute_GetSoftBodyImpact(HitActor))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		}
	}
}

void AMeleeWeapon::EmptyIgnoreActors()
{
	IgnoreActors.Empty();
}