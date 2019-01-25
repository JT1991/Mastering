// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeAIController.generated.h"

/**
 * 
 */
UCLASS()
class MASTERING_API AMeleeAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMeleeAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetChange(class AMasteringCharacter* Target);

	UFUNCTION(BlueprintCallable)
	class AMasteringCharacter* GetTarget();

	UFUNCTION(BlueprintCallable)
	void SetReturningHome();

	UFUNCTION(BlueprintCallable)
	void OnReturnedHome();

	virtual void BeginPlay() override;

	//AI Targeting Controls
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	class USphereComponent* HearingSphere;
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	class USphereComponent* StealthHearingSphere;
	UPROPERTY(VisibleAnywhere, Category = "Targeting")
	class USphereComponent* SightSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float HearingRadius = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float StealthHearingRadius = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float SightRadius = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float SightAngle = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float AttackRadius = 120.f;

protected:
	UFUNCTION()
	void OnHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnStealthHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor * Other, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnSightOverlap(UPrimitiveComponent* OverlappedComp, AActor * Other, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void SetPotentialTarget(AActor* Other);
	
	UPROPERTY()
	AMasteringCharacter* CurrentTarget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FVector HomeLocation;

};
