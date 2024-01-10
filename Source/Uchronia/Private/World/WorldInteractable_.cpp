// Retropsis @ 2023-2024


#include "World/WorldInteractable_.h"

#include "Character/PlayerCharacter.h"
#include "Components/SphereComponent.h"

AWorldInteractable_::AWorldInteractable_()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);

	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractableMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	// InteractableMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	InteractableMesh->SetSimulatePhysics(true);
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(InteractableMesh);
	CollisionSphere->SetSphereRadius(CollisionRadius);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AWorldInteractable_::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWorldInteractable_::OnBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AWorldInteractable_::OnEndOverlap);
}

void AWorldInteractable_::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AWorldInteractable_::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWorldInteractable_::Interact(APlayerCharacter* PlayerCharacter)
{
}
