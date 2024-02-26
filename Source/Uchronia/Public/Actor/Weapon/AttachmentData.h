#pragma once

#include "CoreMinimal.h"
#include "AttachmentData.generated.h"

class UAttachmentComponent;

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
struct FAttachmentSlotData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttachmentType AttachmentType = EAttachmentType::EAT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> AttachedObject = nullptr;
};

USTRUCT()
struct FAttachmentInitializationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCapacity = 0;
};

USTRUCT(BlueprintType)
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
	TSubclassOf<UAttachmentComponent> AttachmentComponentClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> AttachmentMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> AttachmentIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAttachmentInitializationData AttachmentInitializationData = FAttachmentInitializationData();
};
