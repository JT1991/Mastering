// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringInventory.h"
#include "MasteringCharacter.h"


#define UNLIMITED_AMMO -1

// Sets default values for this component's properties
UMasteringInventory::UMasteringInventory()
{
	PrimaryComponentTick.bCanEverTick = true;

	MyOwner = Cast<AMasteringCharacter>(GetOwner());
	check(GetOwner() == nullptr || MyOwner != nullptr);
}


// Called when the game starts
void UMasteringInventory::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeapon != nullptr)
	{
		//Since we don't use pick-up for this weapon, given power of 0.
		AddWeapon(DefaultWeapon, UNLIMITED_AMMO, 0);
	}
}

void UMasteringInventory::SelectBestWeapon()
{
	int highestWeaponPower = CurrentWeaponPower;
	TSubclassOf<class AMasteringWeapon> bestWeapon = nullptr;
	for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
	{
		//TODO add criteria for selecting weapon
		const FWeaponProperties &currentProps = *WeaponIt;

		//skip weapons with no ammo, ammo of -1 means unlimited
		if (currentProps.Ammo == 0)
			continue;
		if (currentProps.WeaponPower > highestWeaponPower)
		{
			highestWeaponPower = currentProps.WeaponPower;
			CurrentWeaponPower = highestWeaponPower;
			bestWeapon = currentProps.WeaponClass;
		}
	}

	if (bestWeapon != nullptr)
	{
		SelectWeapon(bestWeapon);
	}
}

void UMasteringInventory::SelectWeapon(TSubclassOf<class AMasteringWeapon> Weapon)
{
	MyOwner->EquipWeapon(Weapon);
}

void UMasteringInventory::AddWeapon(TSubclassOf<class AMasteringWeapon> Weapon, int AmmoCount, uint8 WeaponPower)
{
	for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
	{
		FWeaponProperties &currentProps = *WeaponIt;
		if (currentProps.WeaponClass == Weapon)
		{
			checkSlow(AmmoCount >= 0);
			currentProps.Ammo += AmmoCount;
			return; 
		}
	}
	
	FWeaponProperties weaponProps;
	weaponProps.WeaponClass = Weapon;
	weaponProps.WeaponPower = WeaponPower;
	weaponProps.Ammo = AmmoCount;

	WeaponsArray.Add(weaponProps);
}

void UMasteringInventory::ChangeAmmo(TSubclassOf<class AMasteringWeapon> Weapon, const int ChangeAmount)
{
	for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
	{
		FWeaponProperties &currentProps = *WeaponIt;
		if (currentProps.WeaponClass == Weapon)
		{
			if (currentProps.Ammo == UNLIMITED_AMMO)
				return;

			currentProps.Ammo = FMath::Clamp(currentProps.Ammo + ChangeAmount, 0, 999);

			if (currentProps.Ammo == 0)//gun is out of ammo
			{
				CurrentWeaponPower = -1; //force to slect any better weapon that has ammo
				SelectBestWeapon();
			}
			
			return; 
		}

	}
}

