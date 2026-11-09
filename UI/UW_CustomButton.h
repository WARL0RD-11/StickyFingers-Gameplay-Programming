// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_CustomButton.generated.h"

//Forward Declarations

class UButton;
class UTextBlock;
class UWidgetAnimation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonClicked);

/**
 * 
 */
UCLASS()
class STICKYFINGERS_API UUW_CustomButton : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable, Category = "Custom Button")
	FOnCustomButtonClicked OnClicked;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Button")
	void SetButtonText(const FText& NewText);

	UFUNCTION()
	void HandleOnClicked();

	UFUNCTION()
	void HandleOnHovered();

	UFUNCTION()
	void HandleOnUnhovered();

	UFUNCTION(BlueprintCallable)
	void SetLocked(bool bInLocked);

	UFUNCTION(BlueprintCallable)
	bool IsLocked() const { return bLocked; }


protected: 
	virtual void NativeConstruct() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> HoverAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock State")
	bool bLocked = false;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	float FocusedScale = 1.15f; // Scale when focused

	UPROPERTY(EditAnywhere, Category = "UI")
	float NormalScale = 1.0f; // Normal scale

	void UpdateButtonScale(float Scale);
};
