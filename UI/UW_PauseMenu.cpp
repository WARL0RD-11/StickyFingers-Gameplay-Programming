// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_PauseMenu.h"
#include "UW_CustomButton.h"
#include "UW_Settings.h"
#include "Kismet/GameplayStatics.h"

#include "UW_PauseMenu.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

void UUW_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	PC = GetOwningPlayer();
	
	PauseButtons.Empty();

	if (MapRevealBox)
	{
		MapRevealBox->SetWidthOverride(0.f);
	}

	if (MapRollEdge)
	{
		FWidgetTransform T = MapRollEdge->GetRenderTransform();
		T.Translation.X = 0.f;
		MapRollEdge->SetRenderTransform(T);
	}

	if(ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UUW_PauseMenu::OnResumeClicked);
		ResumeButton->SetButtonText(FText::FromString("Resume"));
		PauseButtons.Add(ResumeButton);
	}
	
	if(StuckButton)
	{
		StuckButton->OnClicked.AddDynamic(this, &UUW_PauseMenu::OnStuckClicked);
		StuckButton->SetButtonText(FText::FromString("Stuck?"));
		PauseButtons.Add(StuckButton);
	}

	if(ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UUW_PauseMenu::OnReturnClicked);
		ReturnButton->SetButtonText(FText::FromString("Return to Title"));
		PauseButtons.Add(ReturnButton);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UUW_PauseMenu::OnExitClicked);
		ExitButton->SetButtonText(FText::FromString("Exit to Desktop"));
		PauseButtons.Add(ExitButton);
	}
	
	CurrentButtonIndex = 0;
	OnConstructUICall();

	

	FTimerHandle FocusHandle;
	GetWorld()->GetTimerManager().SetTimer(FocusHandle, this, &UUW_PauseMenu::FocusCurrentButton, 0.05f, false);

	UpdateMapImage();
}

void UUW_PauseMenu::UpdateMapImage()
{
	if (!MapImage)
	{
		return;
	}

	const FName CurrentLevelName(*UGameplayStatics::GetCurrentLevelName(this, true));

	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		FString::Printf(TEXT("Current Level: %s"), *CurrentLevelName.ToString())
	);

	if (UTexture2D** FoundTexture = LevelToTextureMap.Find(CurrentLevelName))
	{
		if (*FoundTexture)
		{
			MapImage->SetBrushFromTexture(*FoundTexture);
			return;
		}
	}

	if (DefaultMapTexture)
	{
		MapImage->SetBrushFromTexture(DefaultMapTexture);
	}
}

void UUW_PauseMenu::FocusCurrentButton()
{
	BaseFocusCurrentButton(PauseButtons);
}

FReply UUW_PauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Enter || Key == EKeys::Virtual_Back)
	{
		OnResumeClicked();
		return FReply::Handled();
	}


	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUW_PauseMenu::OnResumeClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetPause(false);
		RemoveFromParent();
	}
}

void UUW_PauseMenu::OnStuckClicked()
{
	OnStuckClickedBP();
}

void UUW_PauseMenu::OnReturnClicked()
{
	UGameplayStatics::OpenLevel(this, LevelName);
}

void UUW_PauseMenu::OnExitClicked()
{
	if (!PC) return;

	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}