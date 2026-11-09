// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_LoadingScreen.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UUW_LoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	SetRootOpacity(0.0f);

	PlayFadeIn(1.0f);
	
	if (LoadingText)
	{
		LoadingText->SetText(FText::FromString(BaseLoadingString));
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TextTimerHandle,
			this,
			&UUW_LoadingScreen::AdvanceLoadingDots,
			0.35f,
			true
		);
	}
}

void UUW_LoadingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bFadeActive) return;

	FadeElapsed += InDeltaTime;
	const float T = (FadeDuration <= 0.0f) ? 1.0f : FMath::Clamp(FadeElapsed / FadeDuration, 0.0f, 1.0f);

	const float Smooth = T * T * (3.0f - 2.0f * T);
	SetRootOpacity(Smooth);

	//if (T >= 1.0f)
	//{
	//	bFadeActive = false;
	//}
}

void UUW_LoadingScreen::PlayFadeIn(float InDuration)
{
	FadeDuration = FMath::Max(0.01f, InDuration);
	FadeElapsed = 0.0f;
	bFadeActive = true;

	SetRootOpacity(0.0f);
}

void UUW_LoadingScreen::StopLoadingText()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TextTimerHandle);
	}

	if (LoadingText)
	{
		LoadingText->SetText(FText::FromString(TEXT("Loading...")));
	}
}

void UUW_LoadingScreen::AdvanceLoadingDots()
{
	if (!LoadingText) return;

	DotCount = (DotCount + 1) % 4; // 0..3
	FString Dots;
	for (int32 i = 0; i < DotCount; ++i)
	{
		Dots += TEXT(".");
	}

	LoadingText->SetText(FText::FromString(BaseLoadingString + Dots));
}

void UUW_LoadingScreen::SetRootOpacity(float Alpha)
{
	Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

	if (RootBorder)
	{
		RootBorder->SetContentColorAndOpacity(FLinearColor(1, 1, 1, Alpha));
	}
	else
	{
		SetRenderOpacity(Alpha);
	}
}