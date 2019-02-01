// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MasteringWeapon.h"
#include "MasteringInventory.generated.h"

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()
public:
	FWeaponProperties() {};
	
	FWeaponProperties(TSubclassOf<class AMasteringWeapon> Class, UTexture2D* Icon, int Power, int AmmoCount) :
		WeaponClass(Class),
		InventoryIcon(Icon),
		WeaponPower(Power),
		Ammo(AmmoCount)
	{ }

	bool operator==(const FWeaponProperties& Other) const
	{
		return Other.WeaponClass == WeaponClass;
	}
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class AMasteringWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly)
	class UTexture2D* InventoryIcon;

	UPROPERTY(BlueprintReadOnly)
	int WeaponPower;

	UPROPERTY(BlueprintReadOnly)
	int Ammo;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERING_API UMasteringInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMasteringInventory();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AMasteringWeaponPickup> DefaultWeaponPickup;

	virtual void AddDefaultWeapon();

	/** Choose best werapon of those available*/
	void SelectBestWeapon();

	/**Select a weapon from inventory */
	void SelectWeapon(FWeaponProperties Weapon);
	
	/** Find current weapon's index */
	int FindCurrentWeaponIndex() const;

	/** Go "up" one weapon in inventory*/
	void SelectNextWeapon();

	/** Go "down" one weapon in inventory*/
	void SelectPreviousWeapon();

	/**Add weapon to inventory list */
	void AddWeapon(const FWeaponProperties &Properties);

	/**Get currently selected weapon */
	FORCEINLINE TSubclassOf<class AMasteringWeapon> GetCurrentWeapon() const { return CurrentWeapon; }

	FORCEINLINE TArray<FWeaponProperties>& GetWeaponsArray() { return WeaponsArray; }

	FORCEINLINE int GetCurrentWeaponPower() const { return CurrentWeaponPower; }

	FORCEINLINE class AMasteringCharacter* GetOwningCharacter() const { return MyOwner; }

	/** Change weapon ammo count */
	void ChangeAmmo(TSubclassOf<class AMasteringWeapon> Weapon, const int ChangeAmount);

	void CopyFromOther(UMasteringInventory *Other, AMasteringCharacter* ownerOverride = nullptr);

	void Reset();

	void SetupToCurrent();

	DECLARE_EVENT_OneParam(UMasteringInventory, FSelectedWeaponChanged, FWeaponProperties)
	FSelectedWeaponChanged OnSelectedWeaponChanged;

	DECLARE_EVENT_OneParam(UMasteringInventory, FWeaponAdded, FWeaponProperties)
	FSelectedWeaponChanged OnWeaponAdded;

	DECLARE_EVENT_OneParam(UMasteringInventory, FWeaponRemoved, FWeaponProperties)
	FSelectedWeaponChanged OnWeaponRemoved;

protected:
	TArray <FWeaponProperties> WeaponsArray;
	TSubclassOf<class AMasteringWeapon> CurrentWeapon;
	int CurrentWeaponPower = -1;
	class AMasteringCharacter* MyOwner;
};
