// Retropsis @ 2023-2024

#include "HUD/PlayerHUD.h"
#include "Character/PlayerCharacter.h"
#include "UI/Widget/BaseUserwidget.h"
#include "UI/Widget/InteractionWidget.h"
#include "UI/Widget/MainMenu.h"
#include "UI/WidgetController/OverlayWidgetController.h"

APlayerHUD::APlayerHUD()
{
	
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(MainMenuClass, TEXT("Main Menu Class missing, fill out BP_PlayerHUD"));
	checkf(InteractionWidgetClass, TEXT("Interaction Widget Class missing, fill out BP_PlayerHUD"));

	MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
	MainMenuWidget->AddToViewport(5);
	MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPawn()))
	{
		MainMenuWidget->InventoryComponent = PlayerCharacter->GetInventoryComponent();
	}
	
	InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
	InteractionWidget->AddToViewport(-1);
	InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class missing, fill out BP_PlayerHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class missing, fill out BP_PlayerHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UBaseUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	
	WidgetController->BroadcastInitialValues();
	
	OverlayWidget->AddToViewport();
}

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if(GEngine)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		const float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if(IsValid(HUDPackage.Crosshair_Center))
		{
			const FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.Crosshair_Center, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(IsValid(HUDPackage.Crosshair_Left))
		{
			const FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.Crosshair_Left, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(IsValid(HUDPackage.Crosshair_Top))
		{
			const FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.Crosshair_Top, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(IsValid(HUDPackage.Crosshair_Right))
		{
			const FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.Crosshair_Right, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if(IsValid(HUDPackage.Crosshair_Bottom))
		{
			const FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.Crosshair_Bottom, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
	}
}

void APlayerHUD::DrawCrosshair(UTexture2D* Texture, const FVector2D& ViewportCenter, const FVector2D& Spread, const FLinearColor& Color)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X -  (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);
	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y,
		TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f,
		Color
	);
}

/*
* Inventory Widgets
*/
void APlayerHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::ToggleMenu()
{
	if(bIsMenuVisible)
	{
		HideMenu();
		FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void APlayerHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}
