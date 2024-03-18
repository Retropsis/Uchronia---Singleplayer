// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ComponentData.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interaction/ComponentInterface.h"
#include "SKM_ComponentCore.generated.h"

class UNiagaraSystem;
class AComponentCore;
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
	
	UFUNCTION(BlueprintCallable)
	void ToggleComponentPreview(bool bShowPreview, USkeletalMesh* PreviewMesh);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetSmokeSocketTransform() const;
	
	void MountingSplash() const;
	
	UFUNCTION(BlueprintCallable)
	void MountSkeletalComponent(bool bIsMounting, USkeletalMesh* ComponentMesh, TSubclassOf<UAnimInstance> ComponentAnimClass, FName RowName);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleSkeletalComponent();

	UFUNCTION(BlueprintCallable)
	void MountActorComponent(TSubclassOf<AActor> ComponentClass, FName HardPointSocket, FName DataRowName);
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleActorComponent();

	UFUNCTION(BlueprintCallable)
	void SetupEngine(AActor* ComponentActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupFuelGauge();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetupHelm(UAnimInstance* HelmAnimInstance);

	/*
	 * Properties
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Component")
	TObjectPtr< UNiagaraSystem> MountingSplashSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Component")
	TObjectPtr<UMaterialInstance> PreviewMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Component")
	EHardPoint HardPoint = EHardPoint::EHP_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Component")
	FDataTableRowHandle ComponentDataRow;
	
	/*
	 * Instance
	 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AVehicle> OwningVehicle;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UHullComponentCore> OwningHullMesh;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimInstance> ComponentAnimInstance;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AComponentCore> TargetComponentCore;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UHardPointTraceComponent> HardPointTraceComponent;
	
	UFUNCTION(BlueprintCallable) FName GetHardPointSocketName() const { return GetAttachSocketName(); }
	UFUNCTION(BlueprintCallable) void SetOwningHullMesh(UHullComponentCore* HullMesh) { OwningHullMesh = HullMesh; }
	UFUNCTION(BlueprintCallable) void SetOwningVehicle(AVehicle* Vehicle) { OwningVehicle = Vehicle; }
	virtual AVehicle* GetOwningVehicle_Implementation() override;
};
