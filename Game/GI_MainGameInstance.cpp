// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_MainGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "../HeistSaveGame.h"

UGI_MainGameInstance::UGI_MainGameInstance()
{
	KeyboardPrompts.SelectText = FText::FromString("Enter");
	KeyboardPrompts.BackText = FText::FromString("Backspace");
	KeyboardPrompts.PauseText = FText::FromString("P");

	XboxPrompts.SelectText = FText::FromString("A");
	XboxPrompts.BackText = FText::FromString("B");
	XboxPrompts.PauseText = FText::FromString("Menu");

	PlayStationPrompts.SelectText = FText::FromString("X");
	PlayStationPrompts.BackText = FText::FromString("O");
	PlayStationPrompts.PauseText = FText::FromString("Options");

	SwitchPrompts.SelectText = FText::FromString("B");
	SwitchPrompts.BackText = FText::FromString("A");
	SwitchPrompts.PauseText = FText::FromString("+");
}

void UGI_MainGameInstance::Init()
{
	Super::Init();
	CurrentInputDevice = EUIInputDevice::Keyboard;
	LoadProgress();
}

void UGI_MainGameInstance::LoadProgress()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveSlotIndex))
	{
		ProgressData = Cast<UHeistSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveSlotIndex));
	}

	if (!ProgressData)
	{
		ProgressData = Cast<UHeistSaveGame>(UGameplayStatics::CreateSaveGameObject(UHeistSaveGame::StaticClass()));
	}

	if (ProgressData && !ProgressData->UnlockedLevels.Contains(JewelryStoreLevelName))
	{
		ProgressData->UnlockedLevels.Add(JewelryStoreLevelName);
	}
	SaveProgress();
}

void UGI_MainGameInstance::SaveProgress()
{
	if (ProgressData)
	{
		UGameplayStatics::SaveGameToSlot(ProgressData, SaveSlotName, SaveSlotIndex);
	}
}

bool UGI_MainGameInstance::HasPlayedBefore() const
{
	return ProgressData ? ProgressData->bHasPlayedOnce : false;
}

bool UGI_MainGameInstance::IsJewelryStoreCompleted() const
{
	return ProgressData ? ProgressData->bJewelryStoreCompleted : false;
}

bool UGI_MainGameInstance::IsBankHeistCompleted() const
{
	return ProgressData ? ProgressData->bBankHeistCompleted : false;
}

bool UGI_MainGameInstance::IsCasinoHeistCompleted() const
{
	return ProgressData ? ProgressData->bCasinoHeistCompleted : false;
}

const TArray<FName>& UGI_MainGameInstance::GetUnlockedLevels() const
{
	static TArray<FName> EmptyArray;
	return ProgressData ? ProgressData->UnlockedLevels : EmptyArray;	
}

void UGI_MainGameInstance::MarkJewelryStoreCompleted()
{
	if (!ProgressData) return;

	ProgressData->bHasPlayedOnce = true;
	ProgressData->bJewelryStoreCompleted = true;

	if (!ProgressData->UnlockedLevels.Contains(JewelryStoreLevelName))
	{
		ProgressData->UnlockedLevels.Add(JewelryStoreLevelName);
	}

	if (!ProgressData->UnlockedLevels.Contains(NextLevelAfterJewelry))
	{
		ProgressData->UnlockedLevels.Add(NextLevelAfterJewelry);
	}

	SaveProgress();
}

void UGI_MainGameInstance::MarkBankHeistCompleted()
{
	if (!ProgressData) return;

	ProgressData->bBankHeistCompleted = true;

	if (!ProgressData->UnlockedLevels.Contains(BankHeistLevelName))
	{
		ProgressData->UnlockedLevels.Add(BankHeistLevelName);
	}

	if (!ProgressData->UnlockedLevels.Contains(NextLevelAfterBank))
	{
		ProgressData->UnlockedLevels.Add(NextLevelAfterBank);
	}

	SaveProgress();
}

void UGI_MainGameInstance::MarkCasinoHeistCompleted()
{
	if (!ProgressData) return;

	ProgressData->bCasinoHeistCompleted = true;

	if (!ProgressData->UnlockedLevels.Contains(CasinoLevelName))
	{
		ProgressData->UnlockedLevels.Add(CasinoLevelName);
	}

	SaveProgress();
}

void UGI_MainGameInstance::LockBankHeist()
{
	if (!ProgressData) return;

	ProgressData->bJewelryStoreCompleted = false;


	if (ProgressData->UnlockedLevels.Contains(NextLevelAfterJewelry))
	{
		ProgressData->UnlockedLevels.Remove(NextLevelAfterJewelry);
	}

	if (ProgressData->UnlockedLevels.Contains(BankHeistLevelName))
	{
		ProgressData->UnlockedLevels.Remove(BankHeistLevelName);
	}

	SaveProgress();
}

void UGI_MainGameInstance::LockCasinoHeist()
{
	if (!ProgressData) return;

	ProgressData->bBankHeistCompleted = false;

	if (ProgressData->UnlockedLevels.Contains(NextLevelAfterBank))
	{
		ProgressData->UnlockedLevels.Remove(NextLevelAfterBank);
	}

	if (ProgressData->UnlockedLevels.Contains(CasinoLevelName))
	{
		ProgressData->UnlockedLevels.Remove(CasinoLevelName);
	}

	SaveProgress();
}

void UGI_MainGameInstance::UnlockLevel(FName LevelName)
{
	if (!ProgressData) return;

	if (!ProgressData->UnlockedLevels.Contains(LevelName))
	{
		ProgressData->UnlockedLevels.Add(LevelName);
		SaveProgress();
	}
}

bool UGI_MainGameInstance::IsLevelUnlocked(FName LevelName) const
{
	return ProgressData ? ProgressData->UnlockedLevels.Contains(LevelName) : false;
}

void UGI_MainGameInstance::SetDualControllerMode(bool bEnabled)
{
	bIsDualControllerModeEnabled = bEnabled;
}

bool UGI_MainGameInstance::IsDualControllerMode() const
{
	return bIsDualControllerModeEnabled;
}

void UGI_MainGameInstance::FinalizeItemList(TArray<AActor*> items, int time, int value, int count)
{
	lastTime = time;
	lastTotalItemCount = count;
	lastTotalValue = value;
	
	lastItemList.Empty();
	
	for (AActor* item : items)
	{
		if (!IsValid(item)) continue;
		
		TSoftClassPtr<AActor> itemSoftClass(item->GetClass());
		lastItemList.FindOrAdd(itemSoftClass) += 1;
	}
}

void UGI_MainGameInstance::WipeLastHeist()
{
	lastTime = -1;
	lastItemList.Empty();
}

void UGI_MainGameInstance::FinishedTutorial()
{
	finishedTutorial = true;
}


void UGI_MainGameInstance::UpdateInputDeviceFromKey(const FKey& PressedKey)
{

	//-----------------------------------
	// GAMEPAD INPUT
	//-----------------------------------

	if (PressedKey.IsGamepadKey())
	{
		if (CurrentInputDevice != EUIInputDevice::Xbox)
		{
			SetCurrentInputDevice(EUIInputDevice::Xbox);
		}

		return;
	}

	//-----------------------------------
	// KEYBOARD / MOUSE INPUT
	//-----------------------------------

	const bool bKeyboardOrMouse =
		PressedKey.IsMouseButton() ||
		!PressedKey.IsGamepadKey();

	if (bKeyboardOrMouse)
	{
		if (CurrentInputDevice != EUIInputDevice::Keyboard)
		{
			SetCurrentInputDevice(EUIInputDevice::Keyboard);
		}

		return;
	}
}

void UGI_MainGameInstance::SetCurrentInputDevice(EUIInputDevice NewDevice)
{
	if (CurrentInputDevice == NewDevice)
	{
		return;
	}

	CurrentInputDevice = NewDevice;
	OnUIInputDeviceChanged.Broadcast(CurrentInputDevice);
}

EUIInputDevice UGI_MainGameInstance::GetCurrentInputDevice() const
{
	return CurrentInputDevice;
}

FUIActionPrompt UGI_MainGameInstance::GetCurrentPrompts() const
{
	switch (CurrentInputDevice)
	{
	case EUIInputDevice::Keyboard:
		return KeyboardPrompts;

	case EUIInputDevice::Xbox:
		return XboxPrompts;

	case EUIInputDevice::PlayStation:
		return PlayStationPrompts;

	case EUIInputDevice::NintendoSwitch:
		return SwitchPrompts;

	default:
		return KeyboardPrompts;
	}
}

FText UGI_MainGameInstance::GetSelectPrompt() const
{
	return GetCurrentPrompts().SelectText;
}

FText UGI_MainGameInstance::GetBackPrompt() const
{
	return GetCurrentPrompts().BackText;
}

void UGI_MainGameInstance::ResetHeistProgress()
{
	if (!ProgressData) return;

	ProgressData->bHasPlayedOnce = false;
	ProgressData->bJewelryStoreCompleted = false;
	ProgressData->bBankHeistCompleted = false;
	ProgressData->bCasinoHeistCompleted = false;

	ProgressData->UnlockedLevels.Empty();
	ProgressData->UnlockedLevels.Add(JewelryStoreLevelName);

	SaveProgress();
}

FString UGI_MainGameInstance::GetHeistProgressDebugString() const
{
	const bool bJewelryUnlocked = IsLevelUnlocked(JewelryStoreLevelName);
	const bool bBankUnlocked = IsLevelUnlocked(BankHeistLevelName);
	const bool bCasinoUnlocked = IsLevelUnlocked(CasinoLevelName);

	return FString::Printf(
		TEXT("Unlocked[J:%s B:%s C:%s] Completed[J:%s B:%s C:%s]"),
		bJewelryUnlocked ? TEXT("Y") : TEXT("N"),
		bBankUnlocked ? TEXT("Y") : TEXT("N"),
		bCasinoUnlocked ? TEXT("Y") : TEXT("N"),
		IsJewelryStoreCompleted() ? TEXT("Y") : TEXT("N"),
		IsBankHeistCompleted() ? TEXT("Y") : TEXT("N"),
		IsCasinoHeistCompleted() ? TEXT("Y") : TEXT("N")
	);
}

void UGI_MainGameInstance::UpdateUIOwnerFromKeyEvent(const FKeyEvent& InKeyEvent)
{
	const FKey PressedKey = InKeyEvent.GetKey();
	const int32 NewUserIndex = InKeyEvent.GetUserIndex();

	EUIInputDevice NewDevice = EUIInputDevice::Keyboard;

	if (PressedKey.IsGamepadKey())
	{
		NewDevice = EUIInputDevice::Xbox;
	}
	else
	{
		NewDevice = EUIInputDevice::Keyboard;
	}

	const bool bUserChanged = (ActiveUIUserIndex != NewUserIndex);
	const bool bDeviceChanged = (CurrentInputDevice != NewDevice);

	ActiveUIUserIndex = NewUserIndex;
	CurrentInputDevice = NewDevice;

	if (bUserChanged || bDeviceChanged)
	{
		OnUIInputOwnerChanged.Broadcast(CurrentInputDevice, ActiveUIUserIndex);
		OnUIInputDeviceChanged.Broadcast(CurrentInputDevice);
	}
}