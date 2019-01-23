// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringIconsCamera.h"
#include "MasteringWeapon.h"
#include "MasteringWeaponPickup.h"
#include "Classes/Engine/AssetManager.h"
#include "Classes/Engine/ObjectLibrary.h"
#include "Classes/Engine/Blueprint.h"
#include "Classes/Components/MeshComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "HighResScreenshot.h"
#include "DrawDebugHelpers.h"




void AMasteringIconsCamera::BeginPlay()
{
	if (bInitalized)
	{
		return; // BeginPlay called multiple times at level start;
	}
	bInitalized = true;

	CameraComp = GetCameraComponent();

	UWorld* World = GetWorld();
	check(World != nullptr);

	APlayerController* Player = World->GetFirstPlayerController();
	Player->SetCinematicMode(true, true, true, true, true);//Shuts down movement, HUD, etc.
	Player->SetViewTarget(this);
	FString contentPath = FString("/Game/") + WeaponsPath;

	/* Object library used to mass-find our pickup objects in the path */
	static UObjectLibrary* ObjectLibrary = nullptr; 
	ObjectLibrary = UObjectLibrary::CreateLibrary(AMasteringWeaponPickup::StaticClass(), false, GIsEditor);
	ObjectLibrary->AddToRoot();
	ObjectLibrary->bHasBlueprintClasses = true;
	ObjectLibrary->LoadBlueprintAssetDataFromPath(contentPath);

	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	/* Iterate the Object Library list of assets */
	for (auto itr : AssetDatas)
	{
		FSoftObjectPath assetPath(itr.ObjectPath.ToString());
		WeaponBlueprintSoftRefs.Add(assetPath); // Dont explicitly need to soft reference, but good practice.
	}

	//Here we stream in the assets found that are weapon pickups and when done, will call the OnFinished function
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(WeaponBlueprintSoftRefs, FStreamableDelegate::CreateUObject(this, &AMasteringIconsCamera::OnFinishedLoadingAssets));
}

void AMasteringIconsCamera::TakeShot()
{
	UWorld* World = GetWorld();
	check(CurrentWeaponPickup != nullptr);

	UMeshComponent* Mesh = Cast<UMeshComponent>(CurrentWeaponPickup->GetComponentByClass(UMeshComponent::StaticClass()));
	check(Mesh != nullptr);

	Mesh->bForceMipStreaming = true;
	Mesh->SetRenderCustomDepth(true);
	GScreenshotResolutionX = ScreenshotResolutionX;
	GScreenshotResolutionY = ScreenshotResolutionY;
	GetHighResScreenshotConfig().SetHDRCapture(true);
	GetHighResScreenshotConfig().bMaskEnabled = true;
	World->GetGameViewport()->Viewport->TakeHighResScreenShot();

	//this timer is here to wait just one frame and then destroy the current actor
	// and spawn the next one; is destroyed same frame, chance screenshot will not appear

	FTimerHandle SpawnNextTimer;
	World->GetTimerManager().SetTimer(SpawnNextTimer, [this] {
		if (CurrentWeaponIndex >= WeaponBlueprints.Num())
		{
			CurrentWeaponIndex = 0; // we have finished, this will break timer on next loop.
		}
		else
		{
			SpawnAndPlaceNextActor();
		}
	},
	0.001f, false);
}

void AMasteringIconsCamera::SpawnAndPlaceNextActor()
{
	if (CurrentWeaponPickup != nullptr)
		CurrentWeaponPickup->Destroy(); // Destroy asny current existing actor

	CurrentWeaponBlueprint = WeaponBlueprints[CurrentWeaponIndex];//Get the weapon blueprint of our current weapon index.
	check(CurrentWeaponBlueprint != nullptr); // anything not a blueprint should never find it's way into our list

	UWorld* World = GetWorld();

	FRotator Rot(0.0f);
	FVector Trans(0.0f);

	FTransform Transform(Rot, Trans);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeaponPickup = World->SpawnActor<AMasteringWeaponPickup>(CurrentWeaponBlueprint->GeneratedClass, Transform, ActorSpawnParams);
	CurrentWeaponPickup->RotationSpeed = 0.0f; // the ones we use for screenshot should not spin
	check(CurrentWeaponPickup != nullptr);
	
	FVector Pos = ComputeActorLocation();
	CurrentWeaponPickup->SetActorLocation(Pos);
	CurrentWeaponIndex++;
}

/*************
	In this function, we get the bounding extents of the actor, find its longest dimension (X, Y, or Z) and push it back 
	until that longest side is just at the edge of our view frustum. If it turns out a weapon is more wide than it is long,
	we rotate the larger side to face our camera too.
**************/
FVector AMasteringIconsCamera::ComputeActorLocation()
{
	UWorld* World = GetWorld();

	check(CurrentWeaponPickup != nullptr);
	UMeshComponent* Mesh = Cast<UMeshComponent>(CurrentWeaponPickup->GetComponentByClass(UMeshComponent::StaticClass()));

	FVector InPos;
	FVector BoxExtent;
	CurrentWeaponPickup->GetActorBounds(false, InPos, BoxExtent);

	/* uncomment these to view the actor bounding generated for our pick-ups
	FVector CurrentPosition = CurrentWeaponPickup->GetActorLocation();
	FColor fcRandom(FMath::RandRange(64, 255), FMath::RandRange(64, 255), FMath::RandRange(64, 255));
	DrawDebugLine(World, CurrentPosition, CurrentPosition + InPos, fcRandom, false, 20.0f);
	DrawDebugBox(World, CurrentPosition + InPos, 0.5f * BoxExtent, FQuat(ForceInitToZero), fcRandom, false, 20.0f);
	*/

	/* uncomment these to view the mesh bounding imported with the assets (set in 3D modeling software etc)
	FBoxSphereBounds bsMesh = Mesh->Bounds;
	DrawDebugLine(World, CurrentPosition, bsMesh.Origin, fcRandom, false, 20.0f);
	DrawDebugBox(World, bsMesh.Origin, 0.5f*bsMesh.BoxExtent, FQuat(ForceInitToZero), fcRandom, false, 20.0f);
	*/

	const float fX = BoxExtent.X;
	const float fY = BoxExtent.Y;
	const float fZ = BoxExtent.Z;

	if (fX > fY)
	{
		FRotator YawRot(0.0f, 90.0f, 0.0f);
		CurrentWeaponPickup->SetActorRotation(YawRot);
	}

	const float fLongestBoxSide = FMath::Max(fX, FMath::Max(fY, fZ));

	// FOV is the whole frustum FOV, to make a right triangle down its middle, we use half this angle
	const float FOVhalf = 0.5f*CameraComp->FieldOfView;
	const float FOVradians = FOVhalf * PI / 180.0f;
	const float FOVtan = FMath::Tan(FOVradians);
	float XDistance = fLongestBoxSide / FOVtan;
	
	FVector Positioning(XDistance, 0.0f, 0.0f);

	return CurrentWeaponPickup->GetActorLocation() + Positioning - InPos;
}

void AMasteringIconsCamera::OnFinishedLoadingAssets()
{
	UWorld* World = GetWorld();

	//convert soft references to hard references
	for (auto itr = WeaponBlueprintSoftRefs.CreateIterator(); itr; ++itr)
	{
		UBlueprint *BPObj = CastChecked<UBlueprint>((*itr).ResolveObject());
		WeaponBlueprints.Add(BPObj);
	}

	SpawnAndPlaceNextActor(); // Spawns first pickup and increments CurrentWeaponIndex to 1;

	static FTimerHandle ScreenShotTimer;
	World->GetTimerManager().SetTimer(ScreenShotTimer, [=] {
			if (CurrentWeaponIndex == 0) // only way we come in at index 0 is if there are no more pickups to capture
			{
				World->GetTimerManager().ClearTimer(ScreenShotTimer); // stop the looping timer, have the timer manager clear that timer based on the handle you passed.
				if (APlayerController* Player = UGameplayStatics::GetPlayerController(World, 0))
				{
					Player->ConsoleCommand(TEXT("Exit"), true);
					return;
				}
			}
			TakeShot(); // every ShotDelay interval, we'll call TakeShot, which also cues up the next shot when it finishes.
	},
	ShotDelay, true, ShotDelay);
}
