// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_BaseWidget.h"
#include "UW_MainM.generated.h"

//Forward Declarations
class UButton;
class UUW_CustomButton;

/**
 * 
 */
UCLASS()
class STICKYFINGERS_API UUW_MainM : public UUW_BaseWidget
{
	GENERATED_BODY()
	
private:
	int32 CurrentButtonIndex = 0;
	TArray<UUW_CustomButton*> MenuButtons;
	APlayerController* PC = nullptr;	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> PlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> GameModeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> CreditsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUW_CustomButton> ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> BackText;

	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnGameModeClicked();

	UFUNCTION()
	void OnCreditsClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnBackClicked();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> SettingsMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> CreditsMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> GameModeMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> LevelSelectMenu;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> AnimMain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> pTitleScreen;

	virtual void RefreshUI() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FName LevelName = TEXT("MainMenu");

	UFUNCTION()
	void FocusCurrentButton();
};
