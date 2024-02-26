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
	DefaultArmLength = SpringArm->TargetArmLength;
	OnGearChangeDelegate.AddDynamic(this, &ThisClass::SetThrustSpeedByGear);
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVehicle::IncreaseGear()
{
	switch (CurrentGear) {
	case EGears::EST_R: CurrentGear = EGears::EST_N;
		break;
	case EGears::EST_N: CurrentGear = EGears::EST_1;
		break;
	case EGears::EST_1: CurrentGear = EGears::EST_2;
		break;
	case EGears::EST_2: CurrentGear = EGears::EST_3;
		break;
	case EGears::EST_3: CurrentGear = EGears::EST_4;
		break;
	case EGears::EST_4: CurrentGear = EGears::EST_5;
		break;
	case EGears::EST_5: CurrentGear = EGears::EST_6;
		break;
	case EGears::EST_6: CurrentGear = EGears::EST_6;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
	OnGearChangeDelegate.Broadcast(CurrentGear);
}

void AVehicle::DecreaseGear()
{
	switch (CurrentGear) {
	case EGears::EST_R: CurrentGear = EGears::EST_R;
		break;
	case EGears::EST_N: CurrentGear = EGears::EST_R;
		break;
	case EGears::EST_1: CurrentGear = EGears::EST_N;
		break;
	case EGears::EST_2: CurrentGear = EGears::EST_1;
		break;
	case EGears::EST_3: CurrentGear = EGears::EST_2;
		break;
	case EGears::EST_4: CurrentGear = EGears::EST_3;
		break;
	case EGears::EST_5: CurrentGear = EGears::EST_4;
		break;
	case EGears::EST_6: CurrentGear = EGears::EST_5;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
	OnGearChangeDelegate.Broadcast(CurrentGear);
}

void AVehicle::SetThrustSpeedByGear(EGears InCurrentGear)
{
	switch (InCurrentGear) {
	case EGears::EST_R: TargetThrustSpeed = -100.f;
		break;
	case EGears::EST_N: TargetThrustSpeed = 0.f;
		break;
	case EGears::EST_1: TargetThrustSpeed = 200.f;
		break;
	case EGears::EST_2: TargetThrustSpeed = 400.f;
		break;
	case EGears::EST_3: TargetThrustSpeed = 600.f;
		break;
	case EGears::EST_4: TargetThrustSpeed = 600.f;
		break;
	case EGears::EST_5: TargetThrustSpeed = 600.f;
		break;
	case EGears::EST_6: TargetThrustSpeed = 600.f;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
}

void AVehicle::SetNewGear(const EGears NewGear)
{
	CurrentGear = NewGear;
	OnGearChangeDelegate.Broadcast(CurrentGear);
}