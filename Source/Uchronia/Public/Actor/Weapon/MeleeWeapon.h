// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/Weapon.h"
#include "MeleeWeapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	AMeleeWeapon();
	
	UFUNCTION()
	virtual  void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	);

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties|Melee")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties|Melee")
	TObjectPtr<USceneComponent> BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties|Melee")
	TObjectPtr<USceneComponent> BoxTraceEnd;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties|Melee")
	bool bIsMultiHit = false;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

public:
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	void EmptyIgnoreActors();
};
