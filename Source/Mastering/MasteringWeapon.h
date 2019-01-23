// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasteringWeapon.generated.h"

UCLASS()
class MASTERING_API AMasteringWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMasteringWeapon();

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset = FVector(100.0f, 0.0f, 10.0f);

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = Projectile)
	TSubclassOf<class AMasteringProjectile> ProjectileClass;

	/**Muzzle offset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	class USceneComponent* MuzzleLocation;

	/** Location on gun mesh where projectiles should spawn.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;
	*/

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* WeaponMesh;

public:	
	// Fire the weapon
	void Fire(FRotator ControlRotation, class UAnimInstance* AnimInst, class UMasteringInventory* Inventory);


};
