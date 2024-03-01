// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SKM_ComponentCore.generated.h"

class UHullComponentCore;
class AVehicle;
/**
 * 
 */
UCLASS()
class UCHRONIA_API USKM_ComponentCore : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AVehicle> OwningVehicle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHullComponentCore> OwningHullMesh;

	UFUNCTION(BlueprintCallable) void SetOwningHullMesh(UHullComponentCore* HullMesh) { OwningHullMesh = HullMesh; }
};
