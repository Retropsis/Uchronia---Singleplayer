// Retropsis @ 2023-2024

#include "Actor/Weapon/RangeWeapon.h"
#include "Actor/Weapon/Casing.h"
#include "ActorComponents/CombatComponent.h"
#include "Character/PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Player/CharacterPlayerController.h"

ARangeWeapon::ARangeWeapon()
{
	// AmmoContainer = CreateDefaultSubobject<AAmmoContainer>(TEXT("Ammo Container"));
	// const USkeletalMeshSocket* ClipSocket =WeaponMesh->GetSocketByName(FName("ClipSocket"));
	// if(ClipSocket)
	// {
	// 	ClipSocket->AttachActor(AmmoContainer, WeaponMesh);
	// }
}

void ARangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARangeWeapon, Ammo);
}

void ARangeWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if(Owner != nullptr)
	{
		SetHUDAmmo();
	}
}

void ARangeWeapon::Trigger(const FVector& HitTarget)
{
	if(IsValid(FireAnimation))
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if(CasingClass)
	{
		if(const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject")))
		{
			const FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);
			if(UWorld* World = GetWorld())
			{
				World->SpawnActor<ACasing>(
					CasingClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
				);
			}
		}
	}
	SpendRound();
}

void ARangeWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHUDAmmo();
}

void ARangeWeapon::AddRounds(const int32 RoundsToAdd)
{
	Ammo = FMath::Clamp(Ammo + RoundsToAdd, 0, MagCapacity);
	SetHUDAmmo();
}

void ARangeWeapon::OnRep_Ammo()
{
	OwnerCharacter = OwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : OwnerCharacter;
	if(IsValid(OwnerCharacter) && OwnerCharacter->GetCombatComponent() && IsFull())
	{
		OwnerCharacter->GetCombatComponent()->JumpToReloadEnd();
	}
	SetHUDAmmo();
}

void ARangeWeapon::SetHUDAmmo()
{
	OwnerCharacter = OwnerCharacter == nullptr ? Cast<APlayerCharacter>(GetOwner()) : OwnerCharacter;
	if(OwnerCharacter)
	{
		OwnerController = OwnerController == nullptr ? Cast<ACharacterPlayerController>(OwnerCharacter->Controller) : OwnerController;
		if (OwnerController)
		{
			OwnerController->SetHUDWeaponAmmo(Ammo);
		}
	}
}