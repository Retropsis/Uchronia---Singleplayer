// Retropsis @ 2023-2024


#include "World/ShopKeeper.h"

#include "Character/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

AShopKeeper::AShopKeeper()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	ShopKeeperMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShopKeeperMesh"));
	RootComponent = ShopKeeperMesh;
}

void AShopKeeper::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShopKeeper, InventoryItems);
}

void AShopKeeper::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShopKeeper::Interact(APlayerCharacter* PlayerCharacter)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Purple, FString::Printf(TEXT("What ya buyin' ?")));
	if(PlayerCharacter)
	{
		PlayerCharacter->OpenShop(InventoryItems);
	}
}
