// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MasteringHUD.generated.h"

UCLASS()
class AMasteringHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMasteringHUD();

	virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	
	virtual void InitializeInventory(class UMasteringInventory* PlayerInventory);

	virtual void ToggleMainMenu();

protected:

	/** Crosshair asset pointer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* CrosshairTex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMasteringInventoryDisplay> InventoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMainMenuWidget> MainMenuClass;

	class UMasteringInventoryDisplay* InventoryHUD;
	class UMainMenuWidget* MainMenu;
	class UMasteringInventory* Inventory;

	bool bNeedsInventoryInit = true;
};

