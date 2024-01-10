// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DisplayText;

	void SetDisplayText(const FString& TextToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(APawn* InPawn);

protected:
	virtual void NativeDestruct() override;
};
