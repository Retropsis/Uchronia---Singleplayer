#pragma once

#include "CoreMinimal.h"
#include "AttachmentData.generated.h"

UENUM(BlueprintType)
enum class EAttachmentType : uint8
{
	EAT_None UMETA(DisplayName="None"),
	EAT_Magazine UMETA(DisplayName="Magazine"),
	EAT_Bayonet UMETA(DisplayName="Bayonet"),
	EAT_Scope UMETA(DisplayName="Scope"),
	EAT_Strap UMETA(DisplayName="Strap"),
	EAT_Grip UMETA(DisplayName="Grip"),
	EAT_Stock UMETA(DisplayName="Stock"),
	
	EAT_MAX UMETA(DisplayName="DefaultMAX")
};

USTRUCT()
struct FAttachmentData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttachmentType AttachmentType = EAttachmentType::EAT_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachmentName = FName();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachmentSocketName = FName();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> AttachmentMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> AttachmentIcon = nullptr;
};
