// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_BaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UW_CustomButton.h"	
#include "GameFramework/PlayerController.h"
#include "../Game/GI_MainGameInstance.h"

void UUW_BaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OwningPC = GetOwningPlayer();
	CurrentButtonIndex = 0;

	CachedGI = GetGameInstance<UGI_MainGameInstance>();
	if (CachedGI)
	{
		CachedGI->OnUIInputDeviceChanged.AddDynamic(this, &ThisClass::HandleInputDeviceChanged);
		CachedGI->OnUIInputOwnerChanged.AddDynamic(this, &ThisClass::HandleUIOwnerChanged);
	}
	ApplyCursorVisibilityForCurrentInput();
	RefreshUI();
}

void UUW_BaseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUW_BaseWidget::NativeDestruct()
{
	if (CachedGI)
	{
		CachedGI->OnUIInputDeviceChanged.RemoveDynamic(this, &ThisClass::HandleInputDeviceChanged);
		CachedGI->OnUIInputOwnerChanged.RemoveDynamic(this, &ThisClass::HandleUIOwnerChanged);
	}
	Super::NativeDestruct();
}

FReply UUW_BaseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (CachedGI)
	{
		CachedGI->UpdateUIOwnerFromKeyEvent(InKeyEvent);
	}

	InKeyEvent.GetUserIndex();

	const FKey Key = InKeyEvent.GetKey();

	const bool bNavigateUp =
		(Key == EKeys::Up) ||
		(Key == EKeys::Gamepad_DPad_Up) ||
		(Key == EKeys::W);

	const bool bNavigateDown =
		(Key == EKeys::Down) ||
		(Key == EKeys::Gamepad_DPad_Down) ||
		(Key == EKeys::S);

	if (bNavigateUp)
	{
		if (ButtonsArray.Num() > 0)
		{
			CurrentButtonIndex--;

			if (CurrentButtonIndex < 0)
			{
				CurrentButtonIndex = ButtonsArray.Num() - 1;
			}

			BaseFocusCurrentButton(ButtonsArray);
		}

		return FReply::Handled();
	}

	if (bNavigateDown)
	{
		if (ButtonsArray.Num() > 0)
		{
			CurrentButtonIndex++;

			if (CurrentButtonIndex >= ButtonsArray.Num())
			{
				CurrentButtonIndex = 0;
			}

			BaseFocusCurrentButton(ButtonsArray);
		}

		return FReply::Handled();
	}

	//ADD debug message to show key pressed and user index
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UUW_BaseWidget::HandleNavigation(TArray<UUW_CustomButton*> WidgetButtons, int32 m_CurrentButtonIndex)
{
	BaseFocusCurrentButton(WidgetButtons);
	return FReply::Handled();
}

void UUW_BaseWidget::BaseFocusCurrentButton(TArray<UUW_CustomButton*> WidgetButtons)
{
	if (!WidgetButtons.IsValidIndex(CurrentButtonIndex)) return;
	
	if (UUW_CustomButton* Button = WidgetButtons[CurrentButtonIndex])
	{
		if (OwningPC)
		{
			Button->SetUserFocus(OwningPC);
		}

		Button->SetKeyboardFocus();
	}
}

void UUW_BaseWidget::OnConstructUICall()
{
	if (OwningPC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		OwningPC->SetInputMode(InputMode);
	}
	SetIsFocusable(true);
	ApplyCursorVisibilityForCurrentInput();
}

void UUW_BaseWidget::FocusOnWidget(UUserWidget* Widget)
{
	ApplyCursorVisibilityForCurrentInput();

	FInputModeUIOnly InputMode;
	TSharedPtr<SWidget> WidgetToFocus = Widget ? Widget->TakeWidget() : TSharedPtr<SWidget>();
	InputMode.SetWidgetToFocus(WidgetToFocus);
	OwningPC->SetInputMode(InputMode);

	Widget->SetKeyboardFocus();	
}

void UUW_BaseWidget::RefreshUI()
{
	//OVERRIDE IN CHILD CLASSES SUII
}

void UUW_BaseWidget::HandleInputDeviceChanged(EUIInputDevice Device)
{
	ApplyCursorVisibilityForCurrentInput();
	RefreshUI();
}


void UUW_BaseWidget::HandleUIOwnerChanged(EUIInputDevice Device, int32 UserIndex)
{
	RefreshUI();
	ApplyFocusForActiveUser();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Yellow,
			FString::Printf(TEXT("UI Owner Changed -> UserIndex: %d | Device: %d"), UserIndex, (int32)Device)
		);
	}
}

void UUW_BaseWidget::ApplyFocusForActiveUser()
{
	if (!CachedGI)
	{
		return;
	}
	if (OwningPC)
	{
		SetUserFocus(OwningPC);
		SetKeyboardFocus();
		BaseFocusCurrentButton(ButtonsArray);
	}
}


void  UUW_BaseWidget::ApplyCursorVisibilityForCurrentInput()
{
	if (!OwningPC || !CachedGI)
	{
		return;
	}

	const bool bShowCursor = (CachedGI->GetCurrentInputDevice() == EUIInputDevice::Keyboard);
	OwningPC->SetShowMouseCursor(bShowCursor);
	OwningPC->bShowMouseCursor = bShowCursor;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			bShowCursor ? FColor::Green : FColor::Red,
			FString::Printf(TEXT("ApplyCursorVisibility -> Device=%d ShowCursor=%s"),
				(int32)CachedGI->GetCurrentInputDevice(),
				bShowCursor ? TEXT("TRUE") : TEXT("FALSE"))
		);
	}
}
