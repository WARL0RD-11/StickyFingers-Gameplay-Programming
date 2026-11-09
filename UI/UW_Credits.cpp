// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Credits.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "UW_MainM.h"
#include "UW_CustomButton.h"
#include "Engine/World.h"
#include "Components/ScrollBox.h"
#include "../Game/GI_MainGameInstance.h"

void UUW_Credits::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreditsScrollBox)
	{
		FTimerHandle ScrollTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ScrollTimerHandle, this, &UUW_Credits::AutoScroll, 0.01f, true);
	}
	OnConstructUICall();
}

void UUW_Credits::OnBackClicked()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Back to Main Menu"));

	UUserWidget* MainMenu = CreateWidget<UUserWidget>(PC, pMainMenu);
	if (MainMenu)
	{
		RemoveFromParent();
		MainMenu->AddToViewport();
		MainMenu->SetIsFocusable(true);
	}
	FocusOnWidget(MainMenu);
}

void UUW_Credits::AutoScroll()
{
	if (!CreditsScrollBox) return;

	float CurrentOffset = CreditsScrollBox->GetScrollOffset();	
	float MaxOffset = CreditsScrollBox->GetScrollOffsetOfEnd();

	float NewOffset = CurrentOffset + ScrollSpeed * 0.016f;

	if(NewOffset >= MaxOffset)
	{
		NewOffset = 0.0f; // Reset to top
	}

	CreditsScrollBox->SetScrollOffset(NewOffset);
}

FReply UUW_Credits::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

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

void UUW_Credits::RefreshUI()
{
	if (!BackText) return;

	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI) return;

	const FText BackPrompt = GI->GetBackPrompt();
	BackText->SetText(FText::Format(
		FText::FromString("[{0}] Back"),
		BackPrompt
	));
}