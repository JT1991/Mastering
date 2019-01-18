// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MasteringHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "MasteringInventoryDisplay.h"

AMasteringHUD::AMasteringHUD()
{
	
}

void AMasteringHUD::BeginPlay()
{
	if (InventoryClass != nullptr)
	{
		InventoryHUD = CreateWidget<UMasteringInventoryDisplay>(GetOwningPlayerController(), InventoryClass);
		checkSlow(InventoryHUD != nullptr);
		InventoryHUD->AddToViewport();
	}
}

void AMasteringHUD::DrawHUD()
{
	if (bNeedsInventoryInit && Inventory != nullptr)
	{
		if (InventoryHUD != nullptr)
		{
			InventoryHUD->Init(Inventory);
		}

		//Equip best weapon on startup
		Inventory->AddDefaultWeapon();
		Inventory->SelectBestWeapon();

		bNeedsInventoryInit = false;
	}
	Super::DrawHUD();
	// Draw very simple crosshair
	if(CrosshairTex != nullptr)
	{
		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition((Center.X),
			(Center.Y + 20.0f));

		// draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AMasteringHUD::InitializeInventory(class UMasteringInventory* PlayerInventory)
{
	Inventory = PlayerInventory;
}
