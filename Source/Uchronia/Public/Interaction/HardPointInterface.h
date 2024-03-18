// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Vehicle/ComponentData.h"
#include "HardPointInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHardPointInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API IHardPointInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AVehicle* GetOwningVehicle();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetComponentDataRowName() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EHardPoint GetHardPoint() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDocked() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMesh* GetHardPointMesh() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USKM_ComponentCore* GetHardPointComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMesh* GetComponentPreview() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<UAnimInstance> GetAnimClass() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<AActor> GetComponentActorClass() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetHardPointSocketName() const;
};
