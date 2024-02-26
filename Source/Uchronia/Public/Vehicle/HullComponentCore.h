// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "HullComponentCore.generated.h"

class UHardPointTraceComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UHullComponentCore : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UHullComponentCore();

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|HardPoints")
	// TObjectPtr<UHardPointTraceComponent> HardPointTrace;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|HardPoints")
	// TObjectPtr<USceneComponent> SmokeSpawnLocation;
	
};
