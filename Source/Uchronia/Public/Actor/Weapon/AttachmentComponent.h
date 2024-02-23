// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "AttachmentData.h"
#include "Components/SkeletalMeshComponent.h"
#include "AttachmentComponent.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UAttachmentComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attachment Component")
	EAttachmentType AttachmentType = EAttachmentType::EAT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attachment Component")
	FName AttachmentSocket = FName();
};
