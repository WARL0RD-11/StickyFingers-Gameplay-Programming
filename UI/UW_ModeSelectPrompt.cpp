// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ModeSelectPrompt.h"
#include "Components/TextBlock.h"

void UUW_ModeSelectPrompt::SetPromptText(const FString& InText)
{
	if(PromptText)
	{
		PromptText->SetText(FText::FromString(InText));
	}
}

void UUW_ModeSelectPrompt::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowPromptAnim)
	{
		PlayAnimation(ShowPromptAnim);
	}	
}
