// Retropsis @ 2023-2024

#include "Vehicle/FuelComponent.h"
#include "Kismet/KismetMathLibrary.h"

UFuelComponent::UFuelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFuelComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FuelTransferOperation(DeltaTime);
}

void UFuelComponent::FuelTransferOperation(float DeltaTime)
{
	if(IsValid(TargetFuelComponent))
	{
		CurrentFuelQuantity -= FuelTransferRate * DeltaTime;
		TargetFuelComponent->SetCurrentFuelQuantity(TargetFuelComponent->GetCurrentFuelQuantity() + FuelTransferRate * DeltaTime);
		GEngine->AddOnScreenDebugMessage(1235, 2.f, FColor::Orange,
			FString::Printf(TEXT("Container / Target: %f/%f"), CurrentFuelQuantity, TargetFuelComponent->GetCurrentFuelQuantity()));
		
		if(ShouldEndFuelTransfer()) EndFuelTransferOperation();
	}
}

//~ returns a bool if it should transfer or not
bool UFuelComponent::StartFuelTransferOperation(UFuelComponent* InTargetFuelComponent)
{
	if(IsValid(InTargetFuelComponent))
	{
		TargetFuelComponent = InTargetFuelComponent;
		TargetFuelComponent->SetRefuelState(ERefuelState::ERS_In);
		RefuelState = ERefuelState::ERS_Out;
		return true;
	}
	return false;
}

void UFuelComponent::EndFuelTransferOperation()
{
	if(IsValid(TargetFuelComponent))
	{
		TargetFuelComponent->SetRefuelState(ERefuelState::ERS_None);
		TargetFuelComponent = nullptr;
		RefuelState = ERefuelState::ERS_None;
	}
}

//~ return show much it retrieved
float UFuelComponent::RetrieveFuel(const float QueriedAmount)
{
	if(QueriedAmount <= CurrentFuelQuantity)
	{
		CurrentFuelQuantity = FMath::Clamp(CurrentFuelQuantity - QueriedAmount, 0.f, MaxFuelCapacity);
		GEngine->AddOnScreenDebugMessage(1234, 2.f, FColor::Orange, FString::Printf(TEXT("Queried Amount: %f"), QueriedAmount));
		return QueriedAmount;
	}
	if (CurrentFuelQuantity > 0)
	{
		CurrentFuelQuantity = 0;
		GEngine->AddOnScreenDebugMessage(1234, 2.f, FColor::Orange, FString::Printf(TEXT("Queried Amount: %f"), CurrentFuelQuantity));
		return CurrentFuelQuantity;
	}
	GEngine->AddOnScreenDebugMessage(1234, 2.f, FColor::Orange, FString::Printf(TEXT("Fuel Component Empty")));
	return 0.f;
}

bool UFuelComponent::IsFuelEntryCleared()
{
	return RefuelState == ERefuelState::ERS_None || RefuelState == ERefuelState::ERS_Leak /*&& bIsFuelLidOpen*/;
}

bool UFuelComponent::ShouldEndFuelTransfer()
{
	return  UKismetMathLibrary::NearlyEqual_FloatFloat(TargetFuelComponent->GetCurrentFuelQuantity(), TargetFuelComponent->GetMaxFuelCapacity(), CapacityTolerance)
				|| CurrentFuelQuantity <= 0.f;
}

