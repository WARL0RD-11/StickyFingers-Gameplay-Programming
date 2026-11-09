// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_BaseWidget.h"
#include "UW_DualController.generated.h"

//Forward Declarations
class UButton;
class UUW_CustomButton;
/**
 * 
 */
UCLASS()
class STICKYFINGERS_API UUW_DualController : public UUW_BaseWidget
{
	GENERATED_BODY()

private:
	int32 CurrentButtonIndex = 0;
	TArray<UUW_CustomButton*> ControllerButtons;
	APlayerController* PC = nullptr;

	TObjectPtr<class UGI_MainGameInstance> UIGameInstance;
	TObjectPtr<class UUW_ModeSelectPrompt> ModePromptWidget;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> SingleController;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> DualContorller;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> BackText;

	virtual void RefreshUI() override;

	UFUNCTION()
	void OnSingleControllerClicked();

	UFUNCTION()
	void OnDualContorllerClicked();

	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> pMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> pModePrompt;

	UFUNCTION()
	void FocusCurrentButton();


	UFUNCTION()
	void HideModePrompt();
	
};
