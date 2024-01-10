// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "DropSlot.generated.h"

class AWorldItem_;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UDropSlot : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
