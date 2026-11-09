// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_CustomButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UUW_CustomButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UUW_CustomButton::HandleOnClicked);
		Button->OnHovered.AddDynamic(this, &UUW_CustomButton::HandleOnHovered);
		Button->OnUnhovered.AddDynamic(this, &UUW_CustomButton::HandleOnUnhovered);
	}

	SetIsFocusable(true);
}

void UUW_CustomButton::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	Button->OnHovered.Broadcast();
	//HandleOnHovered();
}

void UUW_CustomButton::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	Button->OnUnhovered.Broadcast();
	//HandleOnUnhovered();
}

void UUW_CustomButton::UpdateButtonScale(float Scale)
{
	FWidgetTransform NewTransform;
	NewTransform.Scale = FVector2D(Scale, Scale);
	SetRenderTransform(NewTransform);
}

void UUW_CustomButton::HandleOnClicked()
{
	OnClicked.Broadcast();
}

void UUW_CustomButton::HandleOnHovered()
{
	if(HoverAnimation)
	{
		PlayAnimation(HoverAnimation);
	}

	UpdateButtonScale(FocusedScale);
}

void UUW_CustomButton::HandleOnUnhovered()
{
	if (HoverAnimation)
	{
		PlayAnimation(HoverAnimation, 0.25f, 1, EUMGSequencePlayMode::Reverse);
	}

	UpdateButtonScale(NormalScale);
}

void UUW_CustomButton::SetButtonText(const FText& NewText)
{
	if (ButtonText)
	{
		ButtonText->SetText(NewText);
	}
}

void UUW_CustomButton::SetLocked(bool bInLocked)
{
	bLocked = bInLocked;

	SetRenderOpacity(bLocked ? 0.45f : 1.0f);
	if (ButtonText)
	{
	     ButtonText->SetColorAndOpacity(bLocked ? FSlateColor(FLinearColor::Gray) : FSlateColor(FLinearColor::White));
	}
}
