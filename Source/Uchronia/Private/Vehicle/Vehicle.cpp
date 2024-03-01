// Retropsis @ 2023-2024


#include "Vehicle/Vehicle.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/VehicleComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/PlayerInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vehicle/HullComponentCore.h"
#include "Vehicle/SkimmerMovement.h"
#include "Vehicle/VehicleCore.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	HullMesh = CreateDefaultSubobject<UHullComponentCore>(TEXT("HullMesh"));
	SetRootComponent(HullMesh);

	// VehicleCore = CreateDefaultSubobject<UVehicleCore>(TEXT("VehicleCore"));
	// SkimmerMovementComponent = CreateDefaultSubobject<USkimmerMovement>(TEXT("SkimmerMovementComponent"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	DefaultArmLength = SpringArm->TargetArmLength;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	OnGearChangeDelegate.AddDynamic(this, &ThisClass::SetThrustSpeedByGear);
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(VehicleControlsContext);
	if(UEnhancedInputLocalPlayerSubsystem*  Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		Subsystem->AddMappingContext(VehicleControlsContext, 0);
	}
	
	UPlayerInputComponent*EnhancedInputComponent = CastChecked<UPlayerInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVehicle::Look);
	EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &AVehicle::ExitVehicle);
	EnhancedInputComponent->BindAction(SwitchViewAction, ETriggerEvent::Started, this, &AVehicle::SwitchCameraView);
	EnhancedInputComponent->BindAction(SwitchGearAction, ETriggerEvent::Started, this, &AVehicle::SwitchGear);
	EnhancedInputComponent->BindAction(SwitchEngineAction, ETriggerEvent::Started, this, &AVehicle::SwitchEngine);
}

void AVehicle::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	SpringArm->AddRelativeRotation(UKismetMathLibrary::MakeRotator(0.f, LookAxisVector.Y, LookAxisVector.X));
	// AddControllerYawInput(LookAxisVector.X);
	// AddControllerPitchInput(LookAxisVector.Y);
}

void AVehicle::ExitVehicle(const FInputActionValue& InputActionValue)
{
	ServerExitVehicle();
}

void AVehicle::SwitchCameraView(const FInputActionValue& InputActionValue)
{
	if (bIsFirstPersonView)
	{
		SpringArm->TargetArmLength = DefaultArmLength;
		bIsFirstPersonView = false;
	}
	else
	{
		SpringArm->TargetArmLength = FirstPersonArmLength;
		bIsFirstPersonView = true;
	}
}

void AVehicle::SwitchGear(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<float>() > 0)
	{
		IncreaseGear();
	}
	if (InputActionValue.Get<float>() < 0)
	{
		DecreaseGear();
	}
}

void AVehicle::SwitchEngine(const FInputActionValue& InputActionValue)
{
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVehicle::OccupySeat(APlayerCharacter* PlayerCharacter, ESeatType Seat, bool bToOccupy)
{
	switch (Seat) {
	case ESeatType::EST_Driver:
		bDriverSeatOccupied = bToOccupy;
		DriverCharacter = PlayerCharacter;
		break;
	case ESeatType::EST_Passenger_1:
		bPassengerSeatOccupied = bToOccupy;
		PassengerCharacter = PlayerCharacter;
		break;
	case ESeatType::EST_Passenger_2:
		break;
	case ESeatType::EST_Passenger_3:
		break;
	case ESeatType::EST_Passenger_4:
		break;
	case ESeatType::EST_MAX:
		break;
	default: ;
	}
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

/*
 * Vehicle Interface
 */
ESeatType AVehicle::GetFirstAvailableSeat_Implementation()
{
	return !bDriverSeatOccupied ? ESeatType::EST_Driver : !bPassengerSeatOccupied ? ESeatType::EST_Passenger_1 : ESeatType::EST_None; 
}

bool AVehicle::IsAnySeatAvailable_Implementation()
{
	return !bDriverSeatOccupied || !bPassengerSeatOccupied;
}

AVehicle* AVehicle::GetOwningVehicle_Implementation()
{
	return this;
}

void AVehicle::ServerExitVehicle()
{
	if(DriverCharacter && DriverCharacter->GetVehicleComponent())
	{
		GetController()->Possess(DriverCharacter);
		DriverCharacter->GetVehicleComponent()->ExitVehicle();
		DriverCharacter->GetVehicleComponent()->ServerExitVehicle();
	}
}
