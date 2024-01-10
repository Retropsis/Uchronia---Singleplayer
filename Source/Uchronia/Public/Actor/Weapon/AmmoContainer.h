// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoContainer.generated.h"

class AProjectile;

UENUM(BlueprintType)
enum class EAmmoContainerType : uint8
{
	EACT_Box UMETA(DisplayName="Box"),
	EACT_Magazine UMETA(DisplayName="Magazine"),
	EACT_Clip UMETA(DisplayName="Clip"),
	EACT_Barrel UMETA(DisplayName="Magazine"),
	EACT_Drum UMETA(DisplayName="Drum"),
	
	EACT_MAX UMETA(DisplayName="DefaultMAX")
};

UCLASS()
class UCHRONIA_API AAmmoContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoContainer();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	// TODO: Should be a Data Asset
	UPROPERTY(EditAnywhere, Category="Container Properties")
	TObjectPtr<USkeletalMeshComponent> ContainerMesh;

	UPROPERTY(EditAnywhere, Category="Container Properties")
	FName ContainerName;
	
	UPROPERTY(EditAnywhere, Category="Container Properties")
	uint32 MaxCount = 8;
	uint32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, Category="Container Properties")
	EAmmoContainerType AmmoContainerType = EAmmoContainerType::EACT_Magazine;
	
	UPROPERTY(EditAnywhere, Category="Container Properties")
	TSubclassOf<AProjectile> ProjectileClass;

private:

public:	
	FORCEINLINE TSubclassOf<AProjectile> GetProjectileClass() const { return ProjectileClass; }
};
