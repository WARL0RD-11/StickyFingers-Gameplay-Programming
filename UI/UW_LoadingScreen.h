// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LoadingScreen.generated.h"

/**
 * 
 */

class UTextBlock;
class UBorder;

UCLASS()
class STICKYFINGERS_API UUW_LoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "LoadingScreen")
	void PlayFadeIn(float InDuration = 0.35f);

	UFUNCTION(BlueprintCallable, Category = "LoadingScreen")
	void StopLoadingText();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> RootBorder = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LoadingText = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> FadeIn;

private:
	bool bFadeActive = false;
	float FadeDuration = 0.35f;
	float FadeElapsed = 0.0f;

	FTimerHandle TextTimerHandle;
	int32 DotCount = 0;
	FString BaseLoadingString = TEXT("Loading");

	void AdvanceLoadingDots();
	void SetRootOpacity(float Alpha);
};
