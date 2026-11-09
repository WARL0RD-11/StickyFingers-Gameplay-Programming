// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InputCoreTypes.h"
#include "../UI/UI_InputType.h"
#include "GI_MainGameInstance.generated.h"

/**
 * 
 */

class UHeistSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIInputDeviceChanged, EUIInputDevice, NewDevice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIInputOwnerChanged, EUIInputDevice, Device, int32, UserIndex);

UCLASS()
class STICKYFINGERS_API UGI_MainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "UI User Index")
	int32 ActiveUIUserIndex = 0;
	// Whether the player has enabled some setting or flag (example)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	bool bIsDualControllerModeEnabled = false;

	// Optional helper functions
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetDualControllerMode(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Game State")
	bool IsDualControllerMode() const;

	//Carter additions
	//Map to hold the final list of items stored in the duffel bag when the level is over
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	TMap<TSoftClassPtr<AActor>, int32> lastItemList;

	//Int that just holds the seconds the players took to complete a level.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	int lastTime = -1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	int lastTotalValue;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	int lastTotalItemCount;

	//Takes in a list of AACtor classes alongside the time it took to complete a level and stores it in the gameInstance
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void FinalizeItemList(TArray<AActor*> items, int time, int value, int count);

	//Clear the last heist's data
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void WipeLastHeist();
	
	//Storing the location of the last node that the player(s) selected in level select so that when returning to the level select we move the Van there.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	FVector lastHeistNode;

	//Storing then name of the last heist that was started by the players so that in the results UI it can be used to retry it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game State")
	FName lastHeistName;

	//Stores the state of the players and the tutorial so it isn't shown multiple times
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Game State")
	bool finishedTutorial = false;

	//Called when the tutorial is finished
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void FinishedTutorial();


	//------------------------------------------------------------------------------------------------------------
	//						INPUT AND UI SWITCH HANDLING	
	//------------------------------------------------------------------------------------------------------------
	
	virtual void Init() override;
	UGI_MainGameInstance();

	UPROPERTY(BlueprintAssignable, Category = "UI Input")
	FOnUIInputDeviceChanged OnUIInputDeviceChanged;

	UPROPERTY(BlueprintAssignable)
	FOnUIInputOwnerChanged OnUIInputOwnerChanged;

	UFUNCTION(BlueprintCallable, Category = "UI Input")
	void UpdateInputDeviceFromKey(const FKey& PressedKey);

	UFUNCTION(BlueprintCallable, Category = "UI Input")
	void SetCurrentInputDevice(EUIInputDevice NewDevice);

	UFUNCTION(BlueprintPure, Category = "UI Input")
	EUIInputDevice GetCurrentInputDevice() const;

	UFUNCTION(BlueprintPure, Category = "UI Input")
	FUIActionPrompt GetCurrentPrompts() const;

	UFUNCTION(BlueprintPure, Category = "UI Input")
	FText GetSelectPrompt() const;

	UFUNCTION(BlueprintPure, Category = "UI Input")
	FText GetBackPrompt() const;

	int32 GetActiveUIUserIndex() const { return ActiveUIUserIndex; }


	UFUNCTION(BlueprintCallable)
	void UpdateUIOwnerFromKeyEvent(const FKeyEvent& InKeyEvent);
//------------------------------------------------------------------------------------------------------------
//						LEVEL PROGRESSION	
//------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
	void LoadProgress();

	UFUNCTION(BlueprintCallable)
	void SaveProgress();

	UFUNCTION(BlueprintCallable)
	bool HasPlayedBefore() const;

	UFUNCTION(BlueprintCallable)
	const TArray<FName>& GetUnlockedLevels() const;

	UFUNCTION(BlueprintCallable)
	void UnlockLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	bool IsLevelUnlocked(FName LevelName) const;

	UFUNCTION(BlueprintCallable)
	bool IsJewelryStoreCompleted() const;

	UFUNCTION(BlueprintCallable)
	bool IsBankHeistCompleted() const;

	UFUNCTION(BlueprintCallable)
	bool IsCasinoHeistCompleted() const;

	UFUNCTION(BlueprintCallable)
	void MarkJewelryStoreCompleted();

	UFUNCTION(BlueprintCallable)
	void MarkBankHeistCompleted();

	UFUNCTION(BlueprintCallable)
	void MarkCasinoHeistCompleted();

	UFUNCTION(BlueprintCallable)
	FName GetFirstPlayLevel() const { return JewelryStoreLevelName; }

	UFUNCTION(BlueprintCallable)
	FName GetDefaultNextLevel() const { return NextLevelAfterJewelry; }


	//-------------------LEVEL PROGRESS DEBUG KEYS--------------------
	UFUNCTION(BlueprintCallable)
	void LockBankHeist();

	UFUNCTION(BlueprintCallable)
	void LockCasinoHeist();

	UFUNCTION(BlueprintCallable, Category = "Debug|Progress")
	void ResetHeistProgress();

	UFUNCTION(BlueprintCallable, Category = "Debug|Progress")
	FString GetHeistProgressDebugString() const;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI Input")
	EUIInputDevice CurrentInputDevice = EUIInputDevice::Keyboard;

	FUIActionPrompt KeyboardPrompts;
	FUIActionPrompt XboxPrompts;
	FUIActionPrompt PlayStationPrompts;
	FUIActionPrompt SwitchPrompts;

//------------------------------------------------------------------------------------------------------------
//						LEVEL PROGRESSION	
//------------------------------------------------------------------------------------------------------------
	UPROPERTY()
	UHeistSaveGame* ProgressData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progress")
	FString SaveSlotName = TEXT("LevelProgressSlot");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progress")
	int32 SaveSlotIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
	FName JewelryStoreLevelName = TEXT("JewelryStore");

	UPROPERTY(EditDefaultsOnly, Category = "Level Progression")
	FName BankHeistLevelName = TEXT("BankLevel_Rev2");

	UPROPERTY(EditDefaultsOnly, Category = "Level Progression")
	FName CasinoLevelName = TEXT("Casino");

	UPROPERTY(EditDefaultsOnly, Category = "Level Progression")
	FName NextLevelAfterJewelry = TEXT("BankLevel_Rev2");

	UPROPERTY(EditDefaultsOnly, Category = "Level Progression")
	FName NextLevelAfterBank = TEXT("Casino");

	protected:


};