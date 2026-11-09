// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_BaseWidget.h"
#include "UW_LevelSelection.generated.h"

/**
 * 
 */

class UUW_CustomButton;
class APlayerController;

UCLASS()
class STICKYFINGERS_API UUW_LevelSelection : public UUW_BaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void RefreshUI() override;

	UFUNCTION()
	void FocusCurrentButton();

protected:

	UPROPERTY()
	APlayerController* PC;

	UPROPERTY()
	TArray<UUW_CustomButton*> MenuButtons;

	int32 CurrentButtonIndex = 0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> JewelryStoreButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> BankHeistButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> CasinoHeistButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> BackText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> pMainMenu;

	UFUNCTION()
	void OnJewelryStoreClicked();

	UFUNCTION()
	void OnBankHeistClicked();

	UFUNCTION()
	void OnCasinoHeistClicked();

	UFUNCTION()
	void OnBackClicked();

	void OpenLevelByName(FName LevelName);

	void RefreshLevelLocks();
	void UpdateButtonLockVisual(UUW_CustomButton* Button, bool bUnlocked);
};
