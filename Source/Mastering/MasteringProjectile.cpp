// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MasteringProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AMasteringProjectile::AMasteringProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMasteringProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMasteringProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit physics objects.
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		AController* controller = GetInstigatorController();
		AActor* damager = GetInstigator();
		FDamageEvent DamEvt;

		OtherActor->TakeDamage(DamageAmount, DamEvt, controller, damager != nullptr ? damager : this);

		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}
		else if (Cast<APawn>(OtherActor) != nullptr)
		{
			Destroy();
		}

	}

	EPhysicalSurface surfType = SurfaceType_Default;
	if (OtherComp->GetBodyInstance() != nullptr && OtherComp->GetBodyInstance()->GetSimplePhysicalMaterial() != nullptr)
	{
		surfType = OtherComp->GetBodyInstance()->GetSimplePhysicalMaterial()->SurfaceType;
	}

	USoundCue* cueToPlay = nullptr;
	for (auto physSound : ImpactSounds)
	{
		if (physSound.SurfaceType == surfType)
		{
			cueToPlay = physSound.SoundCue;
			break;
		}
	}

	const float minVelocity = 400.0f; //to stop small bounces at end of projectiles lifetime.
	if (cueToPlay != nullptr && GetVelocity().Size() > minVelocity)
	{
		UGameplayStatics::PlaySoundAtLocation(this, cueToPlay, Hit.Location);
	}
}