// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interaction/ComponentInterface.h"
#include "SKM_ComponentCore.generated.h"

class UHardPointTraceComponent;
class UHullComponentCore;
class AVehicle;
/**
 * 
 */
UCLASS()
class UCHRONIA_API USKM_ComponentCore : public USkeletalMeshComponent, public IComponentInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void InitializeComponentCore(UHardPointTraceComponent* HardPointTrace);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AVehicle> OwningVehicle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHullComponentCore> OwningHullMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHardPointTraceComponent> HardPointTraceComponent;

	UFUNCTION(BlueprintCallable) void SetOwningHullMesh(UHullComponentCore* HullMesh) { OwningHullMesh = HullMesh; }
	UFUNCTION(BlueprintCallable) void SetOwningVehicle(AVehicle* Vehicle) { OwningVehicle = Vehicle; }
	virtual AVehicle* GetOwningVehicle_Implementation() override;
};
