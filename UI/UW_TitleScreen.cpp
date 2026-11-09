// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_TitleScreen.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"
#include "UW_MainM.h"
#include "../Game/GI_MainGameInstance.h"


void UUW_TitleScreen::NativeConstruct()
{
	Super:: NativeConstruct();
	if (BackgroundImage && BackgroundTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(BackgroundTexture);
		Brush.ImageSize = FVector2D(1920.0f, 1080.0f);
		BackgroundImage->SetBrush(Brush);
	}

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	SetIsFocusable(true);
}

void UUW_TitleScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!FlashText) return;

	ElapsedTime += InDeltaTime;

	const float Alpha = ComputeAplha();
	const float ScaleValue = ComputeScale(ElapsedTime);

	FLinearColor NewColor = BaseColor;
	NewColor.A = Alpha;
	FVector2D NewScale(ScaleValue, ScaleValue);

	FlashText->SetColorAndOpacity(FSlateColor(NewColor));
	FlashText->SetRenderScale(NewScale);
}

FReply UUW_TitleScreen::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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
		UUW_MainM* MainMenu = CreateWidget<UUW_MainM>(PC, pMainMenu);
		if (MainMenu)
		{
			RemoveFromParent();
			MainMenu->AddToViewport();
		}

		PC->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;

		FTimerHandle FocusTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FocusTimerHandle, [MainMenu]()
			{
				if (MainMenu && MainMenu->IsValidLowLevelFast())
				{
					MainMenu->SetKeyboardFocus();
					MainMenu->FocusCurrentButton();
				}
			}, 0.01f, false);

		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUW_TitleScreen::RefreshUI()
{
	if (!FlashText) return;

	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI) return;

	const FText SelectPrompt = GI->GetSelectPrompt();
	FlashText->SetText(FText::Format(
		FText::FromString("Press [{0}] to Start"),
		SelectPrompt
	));
}

float UUW_TitleScreen::ComputeAplha() const
{
	const float Raw = FMath::Sin(ElapsedTime * FlashSpeed * 2.0f * PI);
	const float Normalized = (Raw + 1.0f) * 0.5f;
	const float Smooth = FMath::InterpEaseInOut(0.0f, 1.0f, Normalized, 2.0f);
	return FMath::Lerp(MinAlpha, MaxAlpha, Smooth);
}

float UUW_TitleScreen::ComputeScale(float Time) const
{
	const float Raw = FMath::Sin(Time * ScaleSpeed * 2.0f * PI);
	const float Normalized = (Raw + 1.0f) * 0.5f;
	const float Smooth = FMath::InterpEaseInOut(0.0f, 1.0f, Normalized, 2.0f);
	return FMath::Lerp(MinScale, MaxScale, Smooth);
}
