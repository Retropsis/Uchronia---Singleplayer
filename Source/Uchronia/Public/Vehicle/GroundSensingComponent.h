// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GroundSensingComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UGroundSensingComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGroundSensingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	float TraceDistance = 200.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	bool bOnGround = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	FVector HitLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	FVector HitNormal = FVector::ZeroVector;

protected:
};
