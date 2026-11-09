// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_BaseWidget.h"
#include "UW_PauseMenu.generated.h"

/**
 * 
 */

class USizeBox;
class UImage;
class UWidgetAnimation;

UCLASS()
class STICKYFINGERS_API UUW_PauseMenu : public UUW_BaseWidget
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	FName LevelName;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUW_CustomButton> ResumeButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUW_CustomButton> StuckButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUW_CustomButton> ReturnButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUW_CustomButton> ExitButton;

	UFUNCTION(BlueprintCallable)
	void OnResumeClicked();
	
	UFUNCTION()
	void OnStuckClicked();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Menu Events")
	void OnStuckClickedBP();

	UFUNCTION()
	void OnReturnClicked();

	UFUNCTION()
	void OnExitClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> SettingsMenu;

	UFUNCTION()
	void FocusCurrentButton();


	//----MAP---
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> MapRevealBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MapRollEdge = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MapImage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Map Unroll")
	float MapTargetWidth = 567.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Map Unroll")
	float RollEdgeWidth = 32.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map UI")
	TMap<FName, UTexture2D*> LevelToTextureMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map UI")
	UTexture2D* DefaultMapTexture;

	void UpdateMapImage();

private:
	TArray<UUW_CustomButton*> PauseButtons;
	APlayerController* PC = nullptr;
	int32 CurrentButtonIndex = 0;
};
