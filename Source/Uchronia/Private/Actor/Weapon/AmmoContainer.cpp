// Retropsis @ 2023-2024

#include "Actor/Weapon/AmmoContainer.h"

#include "Kismet/GameplayStatics.h"

UAmmoContainer::UAmmoContainer()
{
	bWantsInitializeComponent = true;
}

void UAmmoContainer::InitializeComponent()
{
	Super::InitializeComponent();
	
	if(AmmunitionDataRow.DataTable.Get() && AmmunitionDataRow.RowName.IsValid())
	{
		if(const FAmmunitionData* AmmunitionData =  AmmunitionDataRow.DataTable->FindRow<FAmmunitionData>(AmmunitionDataRow.RowName, TEXT("")))
		{
			SetSkeletalMesh(AmmunitionData->ContainerMesh);
			ContainerSocketName = AmmunitionData->ContainerSocketName;
			MaxCapacity = AmmunitionData->MaxCapacity;
			CurrentCount = AmmunitionData->CurrentCount;
			AmmoContainerType =AmmunitionData->AmmoContainerType;
			AmmunitionType = AmmunitionData->AmmunitionType;
			ProjectileClass = AmmunitionData->ProjectileClass;
			EmptyContainerSound = AmmunitionData->EmptyContainerSound;
			InsertContainerSound = AmmunitionData->InsertContainerSound;
		}
	}
}

void UAmmoContainer::InitializeAttachment(FAttachmentInitializationData InitializationData)
{
	Super::InitializeAttachment(InitializationData);
	CurrentCount = InitializationData.CurrentCount;
	MaxCapacity = InitializationData.MaxCapacity;
}

void UAmmoContainer::BeginPlay()
{
	Super::BeginPlay();
}

bool UAmmoContainer::UseAvailableRound()
{
	if(CurrentCount >= 1)
	{
		CurrentCount--;
		GEngine->AddOnScreenDebugMessage(133, 8.f, FColor::Yellow, FString::Printf(TEXT("Ammo left: %d"), CurrentCount));
		return true;
	}
	GEngine->AddOnScreenDebugMessage(133, 8.f, FColor::Red, FString::Printf(TEXT("Out of ammo")));
	return false;
}

int32 UAmmoContainer::GetCurrentCount()
{
	return CurrentCount;
}

void UAmmoContainer::SetCount(int32 NewCount)
{
	CurrentCount = FMath::Clamp(NewCount, 0, MaxCapacity);
}

void UAmmoContainer::AdjustCount(int32 Amount)
{
	CurrentCount = FMath::Clamp(CurrentCount + Amount, 0, MaxCapacity);
}

void UAmmoContainer::PlayEmptyContainerSound()
{
	if(GetOwner() && EmptyContainerSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EmptyContainerSound, GetOwner()->GetActorLocation());
	}
}
