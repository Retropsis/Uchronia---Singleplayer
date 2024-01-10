// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class UCHRONIA_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CasingMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CasingSound;

	UPROPERTY(EditAnywhere)
	float CasingEjectionImpulse = 8.f;

public:	

};
