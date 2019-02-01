// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelTransitionVolume.h"
#include "MasteringCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LoadSaveLibrary.h"
#include "MasteringCharacter.h"
#include "MasteringGameInstance.h"

void ALevelTransitionVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AMasteringCharacter* Mast = Cast<AMasteringCharacter>(OtherActor);
	UWorld* World = GetWorld();

	if (Mast != nullptr)
	{
		UMasteringGameInstance* gameInst = UMasteringGameInstance::GetInstance();
		gameInst->SetPersistInventory();

		AMasteringCharacter* Char = Cast<AMasteringCharacter>(World->GetFirstPlayerController()->GetPawn());
		check(Char != nullptr);

		gameInst->SetPlayerInventory(Char->GetInventory());

		FString currentMapName = World->GetMapName();
		currentMapName.Split("UEDPIE_0_", nullptr, &currentMapName);//strip out PIE prefix if in PIE
		FString toMapName = TransitionLevel;
		FString fromFile = currentMapName + TEXT("_to_") + toMapName + TEXT(".sav");
		FString toFile = toMapName + TEXT("_to_") + currentMapName + TEXT(".sav");
		gameInst->SetPlayerSafeLocation(GetPlayerSafeLocation());

		//save so we can restore state
		LoadSaveLibrary::SaveGameFile(fromFile, World);

		if (FPaths::FileExists(FPaths::ProjectSavedDir() + toFile))
		{
			LoadSaveLibrary::LoadGameFile(toFile, World);
		}
		else
		{
			UGameplayStatics::OpenLevel(World, *toMapName);
		}

	}
}