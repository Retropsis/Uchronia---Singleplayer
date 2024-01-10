// Retropsis @ 2023-2024


#include "Actor/Weapon/Casing.h"

#include "Kismet/GameplayStatics.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;
	SetLifeSpan(10.f);

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	CasingMesh->AddImpulse(GetActorForwardVector() * CasingEjectionImpulse);
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	
}

void ACasing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(IsValid(CasingSound)) UGameplayStatics::PlaySoundAtLocation(this, CasingSound, GetActorLocation());
	CasingMesh->SetNotifyRigidBodyCollision(false);
}