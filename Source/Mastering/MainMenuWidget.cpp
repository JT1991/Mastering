// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
//#include "SavedActorInterface.h"
#include "MasteringCharacter.h"

TArray<uint8> UMainMenuWidget::BinaryData;

void UMainMenuWidget::Open()
{
	checkSlow(GetVisibility() == ESlateVisibility::Hidden); //only want to open from closed
	SetVisibility(ESlateVisibility::Visible);

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* playerController = World->GetFirstPlayerController();
		if (playerController)
		{
			playerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			playerController->SetInputMode(InputMode);
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}

void UMainMenuWidget::Close()
{
	checkSlow(GetVisibility() == ESlateVisibility::Visible); //only want to open from closed
	SetVisibility(ESlateVisibility::Hidden);

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* playerController = World->GetFirstPlayerController();
		if (playerController)
		{
			playerController->bShowMouseCursor = false;
			FInputModeUIOnly InputMode;
			playerController->SetInputMode(InputMode);
			UGameplayStatics::SetGamePaused(this, false);
		}
	}
}

void UMainMenuWidget::PopulateSaveFiles()
{
	return;
}
void UMainMenuWidget::OnGameLoadedFixup(UWorld* World)
{
	return;
}
void UMainMenuWidget::SaveGame()
{
	return;
}

void UMainMenuWidget::LoadGame(FString SaveFile)
{
	return;
}
/*

void UMainMenuWidget::LoadGame(FString SaveFile)
{
	FString outPath = FPaths::ProjectSavedDir() + SaveFile;
	if (!FFileHelper::LoadFileToArray(BinaryData, *outPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(FString("Game Load Failed: ") + outPath));
	}

	FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
	FromBinary.Seek(0);

	FGameSaveData SaveGameData;
	FromBinary << SaveGameData;

	FromBinary.FlushCache();
	FromBinary.Close();

	FString mapName = SaveGameData.MapName.ToString();
	UWorld *World = GetWorld();

	FString currentMapName = World->GetMapName();
	currentMapName.Spilt("_", nullptr, &currentMapName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	if (mapName == currentMapName)
	{
		World->ServerTravel("?Restart", true);
	}
	else
	{
		UGameplayStatics::OpenLevel(World, *mapName, true);
	}
}

void UMainMenuWidget::SaveGame()
{
	FGameSaveData SaveGameData;
	SaveGameData.Timestamp = FDataTime::Now();

	UWorld *World = GetWorld();
	checkSlow(World != nullptr);

	FString mapName = World->GetMapName();
	mapName.Split("_", nullptr, &mapName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	SaveGameData.MapName = *mapName;

	TArray<AActor> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavedActorInterface::StaticClass(), Actors);

	TArray<FActorSavedData> SavedActors;
	for (auto Actor : Actors)
	{
		FSavedData ActorRecord;
		ActorRecord.MyName = FName(*Actor)
	}

}
*/	
