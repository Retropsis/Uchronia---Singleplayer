// Retropsis @ 2023-2024


#include "HUD/OverheadWidget.h"

#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(const FString& TextToDisplay)
{
	if(DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	const ENetRole LocalRole = InPawn->GetLocalRole();
	const ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ROLE_Authority:
		Role = FString("A");
		break;
	case ROLE_AutonomousProxy:
		Role = FString("AP");
		break;
	case ROLE_SimulatedProxy:
		Role = FString("SP");
		break;
	case ROLE_None:
		Role = FString("None");
		break;
	default: ;
	}
	FString PlayerName;
	if (const APlayerState* PlayerState = InPawn->GetPlayerState())
	{
		PlayerName = PlayerState->GetPlayerName();
	}
	const FString RemoteRoleString = FString::Printf(TEXT("%s: %s"), *Role, *PlayerName);
	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	if (const APlayerState* PlayerState = InPawn->GetPlayerState())
	{
		const FString PlayerName = PlayerState->GetPlayerName();
		SetDisplayText(PlayerName);
	}
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
