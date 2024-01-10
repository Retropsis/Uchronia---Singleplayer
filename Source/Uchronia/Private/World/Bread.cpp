// Retropsis @ 2023-2024


#include "World/Bread.h"

#include "Character/PlayerCharacter.h"

ABread::ABread()
{
	ItemData.ItemClass = StaticClass();
}

void ABread::Use(APlayerCharacter* PlayerCharacter)
{
	if(PlayerCharacter)
	{
		PlayerCharacter->RemoveHunger(HungerToRecover);
	}
}
