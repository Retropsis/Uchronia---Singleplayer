// Retropsis @ 2023-2024


#include "Item/Ammunition.h"

#include "Interaction/LootInterface.h"

void AAmmunition::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority()) AmmoAmount = FMath::RandRange(MinAmount, MaxAmount);
}

void AAmmunition::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(ILootInterface* LootInterface = Cast<ILootInterface>(OtherActor))
	{
		LootInterface->LootAmmunition(WeaponType, AmmoAmount);
	}
	Destroy();
}
