// Retropsis @ 2023-2024


#include "World/WaterBottle.h"

#include "Character/PlayerCharacter.h"

AWaterBottle::AWaterBottle()
{
	ItemData.ItemClass = StaticClass();
}

void AWaterBottle::Use(APlayerCharacter* PlayerCharacter)
{	
	if(PlayerCharacter)
	{
		PlayerCharacter->AddHealth(HealthToRecover);
	}
}
