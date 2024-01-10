// Retropsis @ 2023-2024


#include "World/Item.h"

#include "Character/PlayerCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));;
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ItemMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	RootComponent = ItemMesh;

	ItemData.ItemClass = StaticClass();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Interact(APlayerCharacter* PlayerCharacter)
{
	if(HasAuthority() && IsValid(PlayerCharacter))
	{
		PlayerCharacter->AddInventoryItem(ItemData);
		Destroy();
	}
}

void AItem::Use(APlayerCharacter* PlayerCharacter)
{
	// Implement in derived classes
}

void AItem::SetItemMesh()
{
	ItemMesh->SetStaticMesh(ItemData.ItemMesh);
}
