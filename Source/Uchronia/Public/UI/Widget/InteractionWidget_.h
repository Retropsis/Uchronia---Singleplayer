// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InteractionWidget_.generated.h"

class UProgressBar;
class UTextBlock;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UInteractionWidget_ : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	// void UpdateWidget(const FInteractableData* InteractableData) const;
	void UpdateWidget() const;
	
	UPROPERTY(VisibleAnywhere, Category="Interaction WIdget")
	TObjectPtr<APlayerCharacter> PlayerCharacter;

protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="Interaction WIdget")
	TObjectPtr<UTextBlock> NameText;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="Interaction WIdget")
	TObjectPtr<UTextBlock> ActionText;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="Interaction WIdget")
	TObjectPtr<UTextBlock> QuantityText;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="Interaction WIdget")
	TObjectPtr<UTextBlock> KeyPressedText;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="Interaction WIdget")
	TObjectPtr<UProgressBar> InteractionProgressBar;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="Interaction WIdget")
	float CurrentInteractionDuration;

	UFUNCTION(Category="Interaction WIdget")
	float UpdateInteractionProgress();
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
