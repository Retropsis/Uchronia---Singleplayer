// Retropsis @ 2023-2024

#include "Vehicle/SkimmerMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vehicle/HullComponentCore.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleCore.h"
#include "Vehicle/VehicleData.h"

USkimmerMovement::USkimmerMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkimmerMovement::BeginPlay()
{
	Super::BeginPlay();
}

void USkimmerMovement::InitializeSkimmerMovement(UVehicleCore* InVehicleCore)
{
	OwningVehicle = Cast<AVehicle>(GetOwner());
	if(IsValid(OwningVehicle))
	{
		OwningVehicle->OnGearChangeDelegate.AddDynamic(this, &USkimmerMovement::SetThrustSpeedByGear);
	}
	
	VehicleCore = InVehicleCore;	
	HullReference = Cast<UHullComponentCore>(GetOwner()->GetRootComponent());
}

void USkimmerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkimmerMovement::SolveMovement(const float DeltaTime)
{
	switch (VehicleCore->GetMovementState()) {
	case EMovementState::EMS_None:
		TargetThrustSpeed = 0.f;
		TargetSteeringSpeed = 0.f;
		TargetElevationSpeed = 0.f;
		TargetYawRotationSpeed = 0.f;
		break;
	case EMovementState::EMS_Engine_Off_Ready:
		TargetThrustSpeed = 0.f;
		TargetSteeringSpeed = 0.f;
		TargetElevationSpeed = 0.f;
		TargetYawRotationSpeed = 0.f;
		break;
	case EMovementState::EMS_Engine_On_Idling:
		break;
	case EMovementState::EMS_Engine_On_Moving:
		break;
	case EMovementState::EMS_Engine_On_Easing_In:
		break;
	case EMovementState::EMS_Engine_On_Easing_Out:
		TargetThrustSpeed = 0.f;
		TargetSteeringSpeed = 0.f;
		TargetElevationSpeed = 0.f;
		TargetYawRotationSpeed = 0.f;
		break;
	default: ;
	}
	
	CurrentSteeringSpeedFactor = FMath::FInterpTo(CurrentSteeringSpeedFactor, TargetSteeringSpeed, DeltaTime, 2.f);
	CurrentElevationSpeedFactor = FMath::FInterpTo(CurrentElevationSpeedFactor, TargetElevationSpeed, DeltaTime, 2.f);
	CurrentYawRotationSpeedFactor = FMath::FInterpTo(CurrentYawRotationSpeedFactor, TargetYawRotationSpeed, DeltaTime, 2.f);

	// Set Vehicle World Offset
	const FVector V = (OwningVehicle->GetActorForwardVector() *
		FMath::FInterpTo(CurrentThrustSpeedFactor, TargetThrustSpeed, DeltaTime, 0.5f)) +
		(OwningVehicle->GetActorRightVector() * CurrentSteeringSpeedFactor);
	const FVector PlaneNormal = FVector(0.f, 0.f, 1.f);
	const FVector Projection = UKismetMathLibrary::ProjectVectorOnToPlane(V, PlaneNormal);
	
	DeltaLocation = (Projection + (OwningVehicle->GetActorUpVector() * CurrentElevationSpeedFactor) + Gravity) * DeltaTime;

	FHitResult Hit;
	OwningVehicle->AddActorWorldOffset(DeltaLocation, true, &Hit, ETeleportType::None);
	// OwningVehicle->AddMovementInput(DeltaLocation, 1.f, true);
	
	FVector Start = OwningVehicle->GetActorLocation() + FVector(0.f, 0.f, 200.f);
	UKismetSystemLibrary::DrawDebugSphere(this, Hit.Location, 35.f, 12, FLinearColor::Red, 1.f);


	// Sine Wave Wiggle
	const float TimeSeconds = UGameplayStatics::GetTimeSeconds(this);
	const float Sine = UKismetMathLibrary::Sin(TimeSeconds) * 25.f * DeltaTime;
	// const float LocationZ = FMath::Clamp(OwningVehicle->GetActorLocation().Z + Sine, 40.f, 500.f);
	const float LocationZ = FMath::Clamp(OwningVehicle->GetActorLocation().Z, 40.f, 500.f);
	const FVector Location = FVector(OwningVehicle->GetActorLocation().X, OwningVehicle->GetActorLocation().Y,LocationZ );
	// OwningVehicle->SetActorLocation(Location, true);

	// Set Vehicle Rotation
	const float Roll = CurrentSteeringSpeedFactor * ThrustRollFactor;
	const float Pitch = CurrentThrustSpeedFactor * SteerPitchFactor;
	const float Yaw = OwningVehicle->GetActorRotation().Yaw + (CurrentYawRotationSpeedFactor * DeltaTime);
	const FRotator Rotation = UKismetMathLibrary::MakeRotator(Roll, Pitch, Yaw);
	OwningVehicle->SetActorRotation(Rotation);

	// Update Thrust Speed
	CurrentThrustSpeedFactor = FMath::FInterpTo(CurrentThrustSpeedFactor, TargetThrustSpeed, DeltaTime, 0.5f);

	if(VehicleCore->GetMovementState() == EMovementState::EMS_Engine_On_Easing_Out &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentThrustSpeedFactor, 0.f, 0.01f) &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentElevationSpeedFactor, 0.f, 0.01f) &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentSteeringSpeedFactor, 0.f, 0.01f) &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentYawRotationSpeedFactor, 0.f, 0.01f))
	{
		VehicleCore->SetMovementState(EMovementState::EMS_Engine_Off_Ready);
	}
}

// TODO: Use DataTable to determine each engine power and fuel efficiency
void USkimmerMovement::SetThrustSpeedByGear(const EGears InCurrentGear)
{
	const int32 EngineCount = VehicleCore->GetEngineData();
	switch (InCurrentGear)
	{
	case EGears::EST_R:
		TargetThrustSpeed = EngineCount * -50.f;
		break;
	case EGears::EST_N:
		TargetThrustSpeed = 0.f;
		break;
	case EGears::EST_1:
		TargetThrustSpeed = EngineCount * 100.f;
		break;
	case EGears::EST_2:
		TargetThrustSpeed = EngineCount * 200.f;
		break;
	case EGears::EST_3:
		TargetThrustSpeed = EngineCount * 300.f;
		break;
	case EGears::EST_4:
		TargetThrustSpeed = EngineCount * 300.f;
		break;
	case EGears::EST_5:
		TargetThrustSpeed = EngineCount * 300.f;
		break;
	case EGears::EST_6:
		TargetThrustSpeed = EngineCount * 300.f;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
}