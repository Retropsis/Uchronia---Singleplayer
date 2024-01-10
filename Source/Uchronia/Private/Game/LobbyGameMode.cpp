// Retropsis @ 2023-2024


#include "Game/LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// GameState.Get()
	int32 NumberOfPlayers = GameState->PlayerArray.Num();
	if(NumberOfPlayers == 2)
	{
		if (UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/Overworld?listen"));
		}
	}
}
