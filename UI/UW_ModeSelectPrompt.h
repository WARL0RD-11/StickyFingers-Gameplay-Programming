// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_BaseWidget.h"
#include "UW_ModeSelectPrompt.generated.h"

/**
 * 
 */
UCLASS()
class STICKYFINGERS_API UUW_ModeSelectPrompt : public UUW_BaseWidget
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void SetPromptText(const FString& InText);

protected:
	virtual void NativeConstruct() override;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> PromptText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBackgroundBlur> BlurBg;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<class UWidgetAnimation> ShowPromptAnim;

private:	
	

};
