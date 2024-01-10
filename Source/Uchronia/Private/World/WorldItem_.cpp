// Retropsis @ 2023-2024

#include "World/WorldItem_.h"
#include "ActorComponents/Inventory/InventoryComponent.h"
#include "Character/PlayerCharacter.h"

void AWorldItem_::Interact(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->AddItem(this);
	Destroy();
}

void AWorldItem_::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Player Interfacing works
	if(IInteractionInterface* Interface = Cast<IInteractionInterface>(OtherActor))
	{
		Interface->AddItem(this);
		Destroy();
	}
}

void AWorldItem_::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
}
