#include "UW_DevHeistDebug.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"
#include "../Game/GI_MainGameInstance.h"
#include "UW_CustomButton.h"

void UUW_DevHeistDebug::NativeConstruct()
{
	Super::NativeConstruct();

	PC = GetOwningPlayer();
	MenuButtons.Empty();

	if (ResetProgressButton)
	{
		ResetProgressButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnResetProgressClicked);
		MenuButtons.Add(ResetProgressButton);
	}

	if (CompleteJewelryButton)
	{
		CompleteJewelryButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnCompleteJewelryClicked);
		MenuButtons.Add(CompleteJewelryButton);
	}

	if (CompleteBankButton)
	{
		CompleteBankButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnCompleteBankClicked);
		MenuButtons.Add(CompleteBankButton);
	}

	if (CompleteCasinoButton)
	{
		CompleteCasinoButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnCompleteCasinoClicked);
		MenuButtons.Add(CompleteCasinoButton);
	}

	if (LockBankButton)
	{
		LockBankButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnLockBankClicked);
		MenuButtons.Add(LockBankButton);
	}

	if (LockCasinoButton)
	{
		LockCasinoButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnLockCasinoClicked);
		MenuButtons.Add(LockCasinoButton);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UUW_DevHeistDebug::OnCloseClicked);
		MenuButtons.Add(CloseButton);
	}

	CurrentButtonIndex = 0;

	SetIsFocusable(true);
	OnConstructUICall();

	RefreshDebugText();
	RefreshUI();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			InitialFocusHandle,
			this,
			&UUW_DevHeistDebug::FocusCurrentButton,
			0.05f,
			false
		);
	}
}

void UUW_DevHeistDebug::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UUW_DevHeistDebug::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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
		if (MenuButtons.IsValidIndex(CurrentButtonIndex) && MenuButtons[CurrentButtonIndex])
		{
			MenuButtons[CurrentButtonIndex]->OnClicked.Broadcast();
			return FReply::Handled();
		}
	}

	const bool bBackPressed =
		(Key == EKeys::Escape) ||
		(Key == EKeys::BackSpace) ||
		(Key == EKeys::Virtual_Back);

	if (bBackPressed)
	{
		OnCloseClicked();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUW_DevHeistDebug::OnResetProgressClicked()
{
	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->ResetHeistProgress();
	}

	RefreshDebugText();
	RefreshUI();
	FocusCurrentButton();
}

void UUW_DevHeistDebug::OnCompleteJewelryClicked()
{
	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->MarkJewelryStoreCompleted();
	}

	RefreshDebugText();
	RefreshUI();
	FocusCurrentButton();
}

void UUW_DevHeistDebug::OnCompleteBankClicked()
{
	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->MarkBankHeistCompleted();
	}

	RefreshDebugText();
	RefreshUI();
	FocusCurrentButton();
}

void UUW_DevHeistDebug::OnCompleteCasinoClicked()
{
	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->MarkCasinoHeistCompleted();
	}

	RefreshDebugText();
	RefreshUI();
	FocusCurrentButton();
}

void UUW_DevHeistDebug::OnLockBankClicked()
{
	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->LockBankHeist();
	}

	RefreshDebugText();
	RefreshUI();
	FocusCurrentButton();
}

void UUW_DevHeistDebug::OnLockCasinoClicked()
{
	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		GI->LockCasinoHeist();
	}

	RefreshDebugText();
	RefreshUI();
	FocusCurrentButton();
}

void UUW_DevHeistDebug::OnCloseClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetPause(false);
		RemoveFromParent();
	}
}

void UUW_DevHeistDebug::RefreshDebugText()
{
	if (!StatusText)
	{
		return;
	}

	if (UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>())
	{
		StatusText->SetText(FText::FromString(GI->GetHeistProgressDebugString()));
	}
}

void UUW_DevHeistDebug::RefreshUI()
{
	UGI_MainGameInstance* GI = GetGameInstance<UGI_MainGameInstance>();
	if (!GI)
	{
		return;
	}

	if (ButtonText)
	{
		const FText SelectPrompt = GI->GetSelectPrompt();
		ButtonText->SetText(FText::Format(
			FText::FromString("[{0}] Select"),
			SelectPrompt
		));
	}

	if (BackText)
	{
		const FText BackPrompt = GI->GetBackPrompt();
		BackText->SetText(FText::Format(
			FText::FromString("[{0}] Back"),
			BackPrompt
		));
	}
}

void UUW_DevHeistDebug::FocusCurrentButton()
{
	BaseFocusCurrentButton(MenuButtons);
}