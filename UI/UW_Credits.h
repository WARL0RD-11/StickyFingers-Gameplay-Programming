// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_BaseWidget.h"
#include "UW_Credits.generated.h"

// Forward Declarations

class UButton;
class UWidgetSwitcher;
class UTextBlock;
class UWidgetAnimation;
class UUW_CustomButton;

/**
 * 
 */
UCLASS()
class STICKYFINGERS_API UUW_Credits : public UUW_BaseWidget
{
	GENERATED_BODY()

private:
	void AutoScroll();

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> pMainMenu;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> ScrollAnim;

	UPROPERTY(meta = (BindWidget))	
	TObjectPtr<class UScrollBox> CreditsScrollBox;	

	virtual void RefreshUI() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> BackText;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scrolling")
	float ScrollSpeed = 20.0f;
	
};
