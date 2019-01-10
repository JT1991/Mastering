// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MasteringWeapon.h"
#include "MasteringInventory.generated.h"


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
	void SelectBestWeapon(class AMasteringCharacter *Player);

	/**Select a weapon from inventory */
	void SelectWeapon(class AMasteringCharacter *Player, TSubclassOf<AMasteringWeapon> Weapon);
	
	/**Add weapon to inventory list */
	void AddWeapon(TSubclassOf<class AMasteringWeapon> Weapon);

	/**Add default weapon that may be set */
	void AddDefaultWeapon();

	/**Get currently selected weapon */
	FORCEINLINE TSubclassOf<class AMasteringWeapon> GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	TArray <TSubclassOf<class AMasteringWeapon>> WeaponsArray;
	TSubclassOf<class AMasteringWeapon> CurrentWeapon;
};
