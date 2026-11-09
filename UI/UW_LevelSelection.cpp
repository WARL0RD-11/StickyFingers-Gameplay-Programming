// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_LevelSelection.h"
#include "UW_CustomButton.h"
#include "Kismet/GameplayStatics.h"
#include "../Game/GI_MainGameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "InputCoreTypes.h"
#include "Components/TextBlock.h"

void UUW_LevelSelection::NativeConstruct()
{
	Super::NativeConstruct();	

	PC = GetOwningPlayer();
	MenuButtons.Empty();

	if (JewelryStoreButton)
	{
		JewelryStoreButton->OnClicked.AddDynamic(this, &UUW_LevelSelection::OnJewelryStoreClicked);
		JewelryStoreButton->SetButtonText(FText::FromString("Jewelry Store"));
		MenuButtons.Add(JewelryStoreButton);
	}

	if (BankHeistButton)
	{
		BankHeistButton->OnClicked.AddDynamic(this, &UUW_LevelSelection::OnBankHeistClicked);
		BankHeistButton->SetButtonText(FText::FromString("Bank Heist"));
		MenuButtons.Add(BankHeistButton);
	}

	if (CasinoHeistButton)
	{
		CasinoHeistButton->OnClicked.AddDynamic(this, &UUW_LevelSelection::OnCasinoHeistClicked);
		CasinoHeistButton->SetButtonText(FText::FromString("Casino Heist"));
		MenuButtons.Add(CasinoHeistButton);
	}

	RefreshLevelLocks();

	CurrentButtonIndex = INDEX_NONE;
	for (int32 i = 0; i < MenuButtons.Num(); ++i)
	{
		if (MenuButtons[i] && MenuButtons[i]->GetIsEnabled())
		{
			CurrentButtonIndex = i;
			break;
		}
	}

	OnConstructUICall();

	if (CurrentButtonIndex != INDEX_NONE && GetWorld())
	{
		FTimerHandle FocusHandle;
		GetWorld()->GetTimerManager().SetTimer(FocusHandle, this, &UUW_LevelSelection::FocusCurrentButton, 0.05f, false);
	}
}

void UUW_LevelSelection::RefreshLevelLocks()
{
	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI) return;

	UpdateButtonLockVisual(JewelryStoreButton, GI->IsLevelUnlocked(TEXT("JewelryStore")));
	UpdateButtonLockVisual(BankHeistButton, GI->IsLevelUnlocked(TEXT("BankLevel_Rev2")));
	UpdateButtonLockVisual(CasinoHeistButton, GI->IsLevelUnlocked(TEXT("Casino")));
}

void UUW_LevelSelection::UpdateButtonLockVisual(UUW_CustomButton* Button, bool bUnlocked)
{
	if (!Button) return;

	Button->SetLocked(!bUnlocked);

	// Keep button active in hierarchy, but don't disable it.
	Button->SetIsEnabled(true);
}

FReply UUW_LevelSelection::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->UpdateInputDeviceFromKey(Key);
	}

	const bool bAcceptPressed =
		(Key == EKeys::Enter) ||
		(Key == EKeys::Virtual_Accept);

	if (bAcceptPressed)
	{
		if (MenuButtons.IsValidIndex(CurrentButtonIndex))
		{
			if (UUW_CustomButton* SelectedButton = MenuButtons[CurrentButtonIndex])
			{
				if (!SelectedButton->IsLocked())
				{
					SelectedButton->HandleOnClicked();
				}
			}
			return FReply::Handled();
		}
	}

	const bool bBackPressed =
		(Key == EKeys::BackSpace) ||
		(Key == EKeys::Virtual_Back);

	if (bBackPressed)
	{
		OnBackClicked();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUW_LevelSelection::FocusCurrentButton()
{
	if (!MenuButtons.IsValidIndex(CurrentButtonIndex))
	{
		return;
	}

	if (!MenuButtons[CurrentButtonIndex] || !MenuButtons[CurrentButtonIndex]->GetIsEnabled())
	{
		return;
	}

	BaseFocusCurrentButton(MenuButtons);
}

void UUW_LevelSelection::RefreshUI()
{
	if (!ButtonText) return;

	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI) return;

	const FText SelectPrompt = GI->GetSelectPrompt();
	ButtonText->SetText(FText::Format(
		FText::FromString("[{0}] Select"),
		SelectPrompt
	));

	const FText BackPrompt = GI->GetBackPrompt();
	BackText->SetText(FText::Format(
		FText::FromString("[{0}] Back"),
		BackPrompt
	));
}

void UUW_LevelSelection::OnJewelryStoreClicked()
{
	OpenLevelByName(TEXT("JewelryStore"));
}

void UUW_LevelSelection::OnBankHeistClicked()
{
	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI) return;

	if (!GI->IsLevelUnlocked(TEXT("BankLevel_Rev2")))
	{
		return;
	}
	OpenLevelByName(TEXT("BankLevel_Rev2"));
}

void UUW_LevelSelection::OnCasinoHeistClicked()
{
	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI) return;

	if (!GI->IsLevelUnlocked(TEXT("Casino")))
	{
		return;
	}
	OpenLevelByName(TEXT("Casino"));
}

void UUW_LevelSelection::OnBackClicked()
{
	if (!PC) return;

	UUserWidget* MainMenu = CreateWidget<UUserWidget>(PC, pMainMenu);
	if (MainMenu)
	{
		RemoveFromParent();
		MainMenu->AddToViewport();
		MainMenu->SetIsFocusable(true);
	}
	FocusOnWidget(MainMenu);
}

void UUW_LevelSelection::OpenLevelByName(FName LevelName)
{
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(this, LevelName);
}