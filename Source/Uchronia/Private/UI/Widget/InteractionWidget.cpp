// Retropsis @ 2023-2024


#include "UI/Widget/InteractionWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interaction/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressedText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::EIT_Pickup:
		KeyPressedText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"), InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;
		
	case EInteractableType::EIT_NonPlayableCharacter:
		KeyPressedText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
		
	case EInteractableType::EIT_Device:
		KeyPressedText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
		
	case EInteractableType::EIT_Toggle:
		KeyPressedText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
		
	case EInteractableType::EIT_Container:
		KeyPressedText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		if(InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
		
	default: ;
	}
	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
}

float UInteractionWidget::UpdateInteractionProgress()
{
	return 0.f;
}

