// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MasteringIconsCamera.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API AMasteringIconsCamera : public ACameraActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	virtual void TakeShot();
	virtual void SpawnAndPlaceNextActor();
	virtual FVector ComputeActorLocation();
	virtual void OnFinishedLoadingAssets();

	UPROPERTY(Transient)
	TArray<FSoftObjectPath> WeaponBlueprintSoftRefs;
	UPROPERTY(Transient)
	TArray<class UBlueprint*> WeaponBlueprints;

	UPROPERTY(Transient)
	class UBlueprint* CurrentWeaponBlueprint = nullptr;
	UPROPERTY(Transient)
	class AMasteringWeaponPickup* CurrentWeaponPickup = nullptr;
	UPROPERTY(Transient)
	class UCameraComponent* CameraComp;
	UPROPERTY(Transient)
	bool bInitalized = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponsPath = "FirstPersonCPP/Blueprints/Weapons"; // Path to weapons in project.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShotDelay = 0.4f; // Used for allow a delay between weapon pickup and screenshot taken, to allow better MIP level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScreenshotResolutionX = 256;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScreenshotResolutionY = 256;

	int CurrentWeaponIndex = 0;

};
