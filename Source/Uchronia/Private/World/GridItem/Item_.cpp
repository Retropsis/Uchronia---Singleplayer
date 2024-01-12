// Retropsis @ 2023-2024

#include "World/GridItem/Item_.h"

#include "ActorComponents/Inventory/InventoryComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "World/GridItem/ItemData_.h"

AItem_::AItem_()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetupAttachment(ItemMesh);
	CollisionSphere->SetSphereRadius(CollisionRadius);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void AItem_::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem_::OnBeginOverlap);
	if(ItemData == nullptr)
	{
		ItemData = GetDefaultItemData();
	}
}

UItemData_* AItem_::GetDefaultItemData_Implementation()
{
	return CreateDefaultSubobject<UItemData_>(TEXT("Default Item"));
}

void AItem_::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Emerald, FString::Printf(TEXT("%s"), *PlayerCharacter->GetInventoryComponent()->GetName()));
		if (PlayerCharacter->GetInventoryComponent()->TryAddGridItem(ItemData))
		{
			Destroy();
		}
	}
}

void AItem_::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

