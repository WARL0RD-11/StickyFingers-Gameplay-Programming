// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InputType.h"
#include "UW_BaseWidget.generated.h"

/**
 * 
 */
class UGI_MainGameInstance;

UCLASS(Abstract)
class STICKYFINGERS_API UUW_BaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply HandleNavigation(TArray<UUW_CustomButton*> WidgetButtons, int32 CurrentButtonIndex);

	UFUNCTION()
	void BaseFocusCurrentButton(TArray<UUW_CustomButton*> WidgetButtons);

	void OnConstructUICall();

	void FocusOnWidget(UUserWidget* Widget);

	virtual TArray<UUW_CustomButton*> GetWidgetButtons() const PURE_VIRTUAL(UUW_BaseWidget::GetWidgetButtons, return {};);

	UFUNCTION(BlueprintCallable)
	virtual void RefreshUI();

	UFUNCTION()
	void HandleUIOwnerChanged(EUIInputDevice Device, int32 UserIndex);

	void ApplyFocusForActiveUser();

	UPROPERTY()
	TArray<UUW_CustomButton*> ButtonsArray;

private:
	APlayerController* OwningPC = nullptr;	
	int32 CurrentButtonIndex;

	UPROPERTY()
	UGI_MainGameInstance* CachedGI;

	UFUNCTION()
	void HandleInputDeviceChanged(EUIInputDevice Device);

	void ApplyCursorVisibilityForCurrentInput();


};
