// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_MainM.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UW_Settings.h"
#include "UW_Credits.h"
#include "UW_CustomButton.h"
#include "UW_TitleScreen.h"
#include "Engine/World.h"
#include "../Game/GI_MainGameInstance.h"

void UUW_MainM::NativeConstruct()
{
	Super::NativeConstruct();
	PC = GetOwningPlayer();

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UUW_MainM::OnPlayClicked);
		PlayButton->SetButtonText(FText::FromString("Play"));
		MenuButtons.Add(PlayButton);
	}

	if(GameModeButton)
	{
		GameModeButton->OnClicked.AddDynamic(this, &UUW_MainM::OnGameModeClicked);
		GameModeButton->SetButtonText(FText::FromString("Game Mode"));
		MenuButtons.Add(GameModeButton);
	}

	if(CreditsButton)
	{
		CreditsButton->OnClicked.AddDynamic(this, &UUW_MainM::OnCreditsClicked);
		CreditsButton->SetButtonText(FText::FromString("Credits"));
		MenuButtons.Add(CreditsButton);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UUW_MainM::OnExitClicked);
		ExitButton->SetButtonText(FText::FromString("Exit"));
		MenuButtons.Add(ExitButton);
	}

	CurrentButtonIndex = 0;
	OnConstructUICall();

	ButtonsArray = MenuButtons;

	if(AnimMain)
	{
		PlayAnimation(AnimMain);
	}	

	FTimerHandle FocusHandle;
	GetWorld()->GetTimerManager().SetTimer(FocusHandle, this, &UUW_MainM::FocusCurrentButton, 0.05f, false);
}

void UUW_MainM::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UUW_MainM::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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
			MenuButtons[CurrentButtonIndex]->HandleOnClicked();
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

void UUW_MainM::RefreshUI()
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

void UUW_MainM::FocusCurrentButton()
{
	BaseFocusCurrentButton(MenuButtons);
}

void UUW_MainM::OnPlayClicked()
{
	if (!PC || !LevelSelectMenu ) return;

	UUserWidget* LevelSelect = CreateWidget<UUserWidget>(PC, LevelSelectMenu);
	if (LevelSelect)
	{
		RemoveFromParent();
		LevelSelect->AddToViewport();
	}

	FocusOnWidget(LevelSelect);
}

void UUW_MainM::OnGameModeClicked()
{
	if (!PC || !GameModeMenu) return;

	UUserWidget* GameMode = CreateWidget<UUserWidget>(PC, GameModeMenu);
	if(GameMode)
	{
		RemoveFromParent(); 
		GameMode->AddToViewport();
	}

	FocusOnWidget(GameMode);	
}

void UUW_MainM::OnCreditsClicked()
{
	if (!PC || !CreditsMenu) return;

	UUserWidget* Credits = CreateWidget<UUserWidget>(PC, CreditsMenu);
	if (Credits)
	{
		RemoveFromParent(); 
		Credits->AddToViewport();
	}

	FocusOnWidget(Credits);	
}

void UUW_MainM::OnBackClicked()
{
	if (!PC) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Back to Main Menu"));

	UUserWidget* Title = CreateWidget<UUserWidget>(PC, pTitleScreen);
	if (Title)
	{
		RemoveFromParent();
		Title->AddToViewport();
		Title->SetIsFocusable(true);
	}
	FocusOnWidget(Title);
}

void UUW_MainM::OnExitClicked()
{
	if (!PC) return;

	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}
