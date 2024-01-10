// Retropsis @ 2023-2024


#include "UI/WidgetController/BaseWidgetController.h"

void UBaseWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UBaseWidgetController::BroadcastInitialValues()
{
}

void UBaseWidgetController::BindCallbacksToDependencies()
{
}
