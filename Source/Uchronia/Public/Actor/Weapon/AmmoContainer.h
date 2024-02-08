// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "AmmoContainer.generated.h"

class AProjectile;

UCLASS()
class UCHRONIA_API UAmmoContainer : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:	
	UAmmoContainer();
	virtual void InitializeComponent() override;

	bool UseAvailableRound();
	void SetCount(int32 NewCount);
	void AdjustCount(int32 Amount);
	void PlayEmptyContainerSound();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Container Properties")
	FDataTableRowHandle AmmunitionDataRow;
	
	FName ContainerSocketName;
	uint32 CurrentCount = 0;
	uint32 MaxCapacity = 8;
	EAmmoContainerType AmmoContainerType = EAmmoContainerType::EACT_Magazine;
	EAmmunitionType AmmunitionType = EAmmunitionType::EAT_9x19mm;
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY()
	TObjectPtr<USoundBase> EmptyContainerSound = nullptr;
	UPROPERTY()
	TObjectPtr<USoundBase> InsertContainerSound = nullptr;

private:

public:	
	FORCEINLINE TSubclassOf<AProjectile> GetProjectileClass() const { return ProjectileClass; }
	FORCEINLINE int32 GetCurrentCount() const { return CurrentCount; }
	FORCEINLINE int32 GetMaxCapacity() const { return MaxCapacity; }
	FORCEINLINE bool IsContainerFull() const { return CurrentCount == MaxCapacity; }
	FORCEINLINE bool IsContainerEmpty() const { return MaxCapacity <= 0; }
	FORCEINLINE EAmmunitionType GetAmmunitionType() const { return  AmmunitionType; }
};
