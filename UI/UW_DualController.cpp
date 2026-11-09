// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_DualController.h"
#include "UW_CustomButton.h"
#include "UW_MainM.h"
#include "UW_ModeSelectPrompt.h"
#include "../Game/GI_MainGameInstance.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UUW_DualController::NativeConstruct()
{
	Super::NativeConstruct();
	PC = GetOwningPlayer();

	
	if (SingleController)
	{
		SingleController->OnClicked.AddDynamic(this, &UUW_DualController::OnSingleControllerClicked);
		ControllerButtons.Add(SingleController);
	}

	if(DualContorller)
	{
		DualContorller->OnClicked.AddDynamic(this, &UUW_DualController::OnDualContorllerClicked);
		ControllerButtons.Add(DualContorller);
	}
	CurrentButtonIndex = 0;
	OnConstructUICall();

	FTimerHandle FocusDelayHandle;
	GetWorld()->GetTimerManager().SetTimer(FocusDelayHandle, this, &UUW_DualController::FocusCurrentButton, 0.05f, false);
}

void UUW_DualController::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	
}

FReply UUW_DualController::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	const bool bAcceptPressed =
		(Key == EKeys::Enter) ||
		(Key == EKeys::Virtual_Accept);

	if (bAcceptPressed)
	{
		if (ControllerButtons.IsValidIndex(CurrentButtonIndex))
		{
			ControllerButtons[CurrentButtonIndex]->HandleOnClicked();
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

void UUW_DualController::FocusCurrentButton()
{
	BaseFocusCurrentButton(ControllerButtons);
}

void UUW_DualController::HideModePrompt()
{
	if (ModePromptWidget)
	{
		ModePromptWidget->RemoveFromParent();
	}
}

void UUW_DualController::RefreshUI()
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

void UUW_DualController::OnSingleControllerClicked()
{
	UIGameInstance = Cast<UGI_MainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!UIGameInstance) return;	

	if (ModePromptWidget)
	{
		ModePromptWidget->RemoveFromParent();
	}

	UIGameInstance->SetDualControllerMode(false);
	ModePromptWidget = CreateWidget<UUW_ModeSelectPrompt>(PC, pModePrompt);
	ModePromptWidget->AddToViewport(50);
	ModePromptWidget->SetPromptText("Single Controller Mode Selected");
}

void UUW_DualController::OnDualContorllerClicked()
{
	UIGameInstance = Cast<UGI_MainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!UIGameInstance) return;

	if(ModePromptWidget)
	{
		ModePromptWidget->RemoveFromParent();
	}

	UIGameInstance->SetDualControllerMode(true);
	ModePromptWidget = CreateWidget<UUW_ModeSelectPrompt>(PC, pModePrompt);
	ModePromptWidget->AddToViewport(50);
	ModePromptWidget->SetPromptText("Dual Controller Mode Selected");
}

void UUW_DualController::OnBackClicked()
{
	if (!PC) return;

	HideModePrompt();

	UUserWidget* MainMenu = CreateWidget<UUserWidget>(PC, pMainMenu);
	if (MainMenu)
	{
		RemoveFromParent();
		MainMenu->AddToViewport();
		MainMenu->SetIsFocusable(true);
	}
	FocusOnWidget(MainMenu);
}