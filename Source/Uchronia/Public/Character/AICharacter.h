// Retropsis @ 2023

#pragma once

#include "CoreMinimal.h"
#include "AICharacterData.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AICharacter.generated.h"

class USplineComponent;
class APatrolSpline;
enum class EEnemyStates : uint8;
class ABaseAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API AAICharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAICharacter();
	virtual void PossessedBy(AController* NewController) override;
	
	//~ Enemy Interface
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual FVector FindRandomLocation_Implementation() override;
	virtual bool MoveToLocation_Implementation(FVector ToLocation, float Threshold) override;
	void BuildDiveTrajectory();
	virtual bool ChasePlayer_Implementation() override;
	virtual void MoveAlongSpline_Implementation(USplineComponent* InPatrolSpline) override;
	virtual FVector GetNextPointOnSpline_Implementation() override;
	virtual bool DiveAlongTrajectory_Implementation() override;
	//~ Enemy Interface
	
	/* Combat Interface */
	virtual int32 GetCharacterLevel() override;
	virtual void HitReact(const FVector& ImpactPoint) override;
	virtual void Die() override;
	/* Combat Interface */

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	/*
	 * AI Behavior Variables
	 */
	UPROPERTY(BlueprintReadOnly, Category="AI|Combat")
	bool bHitReacting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Movement")
	float BaseWalkSpeed = 250.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Movement")
	float BaseFlySpeed = 400.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Movement")
	float BaseChaseSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Movement")
	float BaseDiveSpeed = 900.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Movement")
	float DiveCooldown = 10.f;

	UPROPERTY(BlueprintReadOnly, Category="AI|Movement")
	FTimerHandle DivingTimer;

	UFUNCTION(BlueprintCallable)
	void StartDiving();

	UPROPERTY(BlueprintReadOnly, Category="AI|Movement")
	bool bCirclingDirection = false;

protected:
	virtual void BeginPlay() override;

	/*
	 * AI
	 */
	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category="AI")
	TObjectPtr<ABaseAIController> BaseAIController;
	
	//~ Patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	TObjectPtr<APatrolSpline> PatrolSplineActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	float TimeToComplete = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	int32 CurrentSplinePoint = 0;
	
	//~ Combat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI|Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintReadWrite, Category="AI|Combat")
	FVector StartLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite, Category="AI|Combat")
	FVector TargetLocation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Combat")
	float RoamRadius = 5000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Combat")
	float AttackDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Combat")
	EEnemyStates EnemyState = EEnemyStates::EES_Patrol;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Combat")
	EChasingStates ChasingState = EChasingStates::ECS_None;

	UPROPERTY()
	TObjectPtr<APatrolSpline> DiveTrajectory;

	/*
	 * Ability System
	 */
	virtual void InitAbilityActorInfo() override;
	virtual  void InitializeDefaultAttributes() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Critter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
