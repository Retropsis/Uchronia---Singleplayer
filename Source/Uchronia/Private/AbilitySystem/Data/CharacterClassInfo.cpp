// Retropsis @ 2023-2024


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	 return CharacterClassDefaultInformation.FindChecked(CharacterClass);
}
