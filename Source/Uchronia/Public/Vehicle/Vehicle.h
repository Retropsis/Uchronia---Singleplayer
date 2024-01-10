// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vehicle.generated.h"

class AHelm;
class UBoxComponent;

UCLASS()
class UCHRONIA_API AVehicle : public ACharacter
{
	GENERATED_BODY()

public:
	AVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(FVector2D Movement);

	FVector2D Direction;
	
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere)
	float ForwardSpeed = 400.f;
	
	UPROPERTY(EditAnywhere)
	float LateralSpeed = 300.f;
	
	UPROPERTY(EditAnywhere)
	float ReverseSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	USceneComponent* Helm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> HullMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AHelm> Forward;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AHelm> LeftButton;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AHelm> RightButton;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AHelm> Reverse;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AHelm> Stop;

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE void SetDirection(FVector2D NewDirection) { Direction = NewDirection; }
};
