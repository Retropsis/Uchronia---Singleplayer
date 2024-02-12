// Retropsis @ 2023-2024


#include "Vehicle/Vehicle.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Vehicle/HullComponentCore.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	HullMesh = CreateDefaultSubobject<UHullComponentCore>(TEXT("HullMesh"));
	SetRootComponent(HullMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

