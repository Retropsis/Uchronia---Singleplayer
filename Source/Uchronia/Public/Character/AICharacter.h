// Retropsis @ 2023

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AICharacter.generated.h"

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

protected:
	virtual void BeginPlay() override;

	/*
	 * AI
	 */
	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> BaseAIController;

	UPROPERTY(BlueprintReadWrite, Category="AI|Combat")
	TObjectPtr<AActor> CombatTarget;

	/*
	 * Ability System
	 */
	virtual void InitAbilityActorInfo() override;
	virtual  void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Critter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
