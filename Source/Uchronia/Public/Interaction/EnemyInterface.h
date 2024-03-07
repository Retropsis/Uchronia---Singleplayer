// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"
class USplineComponent;

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector FindRandomLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetNextPointOnSpline();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool DiveAlongTrajectory();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool MoveToLocation(FVector ToLocation, float Threshold);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveAlongSpline(USplineComponent* InPatrolSpline);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ChasePlayer();
};
