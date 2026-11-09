#pragma once

#include "CoreMinimal.h"
#include "UW_BaseWidget.h"
#include "UW_DevHeistDebug.generated.h"

class UButton;
class UTextBlock;
class UUW_CustomButton;

UCLASS()
class STICKYFINGERS_API UUW_DevHeistDebug : public UUW_BaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void OnResetProgressClicked();

	UFUNCTION()
	void OnCompleteJewelryClicked();

	UFUNCTION()
	void OnCompleteBankClicked();

	UFUNCTION()
	void OnCompleteCasinoClicked();

	UFUNCTION()
	void OnLockBankClicked();

	UFUNCTION()
	void OnLockCasinoClicked();

	UFUNCTION()
	void OnCloseClicked();

	void RefreshDebugText();
	void RefreshUI();
	void FocusCurrentButton();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> ResetProgressButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> CompleteJewelryButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> CompleteBankButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> CompleteCasinoButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> LockBankButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> LockCasinoButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> CloseButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StatusText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BackText;

	FTimerHandle InitialFocusHandle;
	FTimerHandle RecoverFocusHandle;

private:
	TArray<UUW_CustomButton*> MenuButtons;
	int32 CurrentButtonIndex = 0;
	APlayerController* PC = nullptr;
};