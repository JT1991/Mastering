// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MasteringWeapon.h"
#include "MasteringInventory.generated.h"

USTRUCT()
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	TSubclassOf<class AMasteringWeapon> WeaponClass;
	
	UPROPERTY()
	int WeaponPower;

	UPROPERTY()
	int Ammo;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERING_API UMasteringInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMasteringInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AMasteringWeapon> DefaultWeapon;

	/** Choose best werapon of those available*/
	void SelectBestWeapon();

	/**Select a weapon from inventory */
	void SelectWeapon(TSubclassOf<AMasteringWeapon> Weapon);
	
	/** Find current weapon's index */
	int FindCurrentWeaponIndex() const;

	/** Go "up" one weapon in inventory*/
	void SelectNextWeapon();

	/** Go "down" one weapon in inventory*/
	void SelectPreviousWeapon();

	/**Add weapon to inventory list */
	void AddWeapon(TSubclassOf<class AMasteringWeapon> Weapon, int AmmoCount, uint8 WeaponPower);

	/**Add default weapon that may be set */
	void AddDefaultWeapon();

	/**Get currently selected weapon */
	FORCEINLINE TSubclassOf<class AMasteringWeapon> GetCurrentWeapon() const { return CurrentWeapon; }

	/** Change weapon ammo count */
	void ChangeAmmo(TSubclassOf<class AMasteringWeapon> Weapon, const int ChangeAmount);



protected:
	TArray <FWeaponProperties> WeaponsArray;
	TSubclassOf<class AMasteringWeapon> CurrentWeapon;
	int CurrentWeaponPower = -1;
	class AMasteringCharacter* MyOwner;
};
