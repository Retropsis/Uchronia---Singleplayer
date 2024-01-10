// Retropsis @ 2023-2024


#include "Vehicle/Vehicle.h"

#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Vehicle/Helm.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	HullMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hull Mesh"));
	HullMesh->SetupAttachment(GetRootComponent());

	Helm = CreateDefaultSubobject<USceneComponent>(TEXT("Helm"));
	Helm->SetupAttachment(RootComponent);
	Stop = CreateDefaultSubobject<AHelm>(TEXT("Stop Button"));
	Stop->HelmDirection = EHelmDirection::EHD_Stop;
	
	Forward = CreateDefaultSubobject<AHelm>(TEXT("Forward Button"));
	Forward->HelmDirection = EHelmDirection::EHD_Forward;
	
	LeftButton = CreateDefaultSubobject<AHelm>(TEXT("Left Button"));
	LeftButton->HelmDirection = EHelmDirection::EHD_Left;
	
	RightButton = CreateDefaultSubobject<AHelm>(TEXT("Right Button"));
	RightButton->HelmDirection = EHelmDirection::EHD_Right;
	
	Reverse = CreateDefaultSubobject<AHelm>(TEXT("Reverse Button"));
	Reverse->HelmDirection = EHelmDirection::EHD_Reverse;
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	Forward->SetVehicleReference(this);
	LeftButton->SetVehicleReference(this);
	RightButton->SetVehicleReference(this);
	Reverse->SetVehicleReference(this);
	Stop->SetVehicleReference(this);

	FAttachmentTransformRules Rules{
		EAttachmentRule::SnapToTarget,
		false
	};
	Stop->AttachToComponent(Helm, Rules);
	Forward->AttachToComponent(Helm, Rules);
	LeftButton->AttachToComponent(Helm, Rules);
	RightButton->AttachToComponent(Helm, Rules);
	Reverse->AttachToComponent(Helm, Rules);
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsMoving) Move(Direction);
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVehicle::Move(FVector2D Movement)
{
	FVector InputDirection{Movement.X, Movement.Y, 0.f};

	FVector Start = FVector{GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 300.f};
	// UKismetSystemLibrary::DrawDebugArrow(this, Start, Start + InputDirection * 100.f, 1.f, FLinearColor::Red, 5.f, 1.f);
	
	const double CosTheta = FVector::DotProduct(InputDirection, GetActorForwardVector());
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	
	const FVector CrossProduct = FVector::CrossProduct(InputDirection, GetActorForwardVector());
	// UKismetSystemLibrary::DrawDebugArrow(this, Start, Start + CrossProduct * 100.f, 1.f, FLinearColor::Blue, 5.f, 1.f);

	if(CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	float FlySpeed = ReverseSpeed;

	if(Theta >= -45.f && Theta <= 45.f) FlySpeed = ForwardSpeed;
	else if (Theta >= -135.f && Theta < -45.f) FlySpeed = LateralSpeed;
	else if (Theta >= 45.f && Theta < 135.f) FlySpeed = LateralSpeed;

	GetCharacterMovement()->MaxWalkSpeed = FlySpeed;
	
	const FVector2D InputAxisVector = Movement;
	const FRotator Rotation = /*GetControlRotation()*/GetActorForwardVector().Rotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(InputDirection, Movement.Normalize());
	// AddMovementInput(RightDirection, InputAxisVector.X);
}

