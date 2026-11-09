// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotMachine.h"
#include "SlotReel.h"
#include "SlotChip.h"

ASlotMachine::ASlotMachine()
{
	PrimaryActorTick.bCanEverTick = true;
	ChipSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CHIP SPAWN LOCATION"));
	RootComponent = ChipSpawnPoint;
}

void ASlotMachine::BeginPlay()
{
	Super::BeginPlay();
	AcceptButtonsNow();
}

void ASlotMachine::BindReels()
{
	for (ASlotReel* Reel : Reels)
	{
		if (!Reel) continue;

		Reel->OnReelStopped.RemoveDynamic(this, &ASlotMachine::HandleReelStopped);
		Reel->OnReelStopped.AddDynamic(this, &ASlotMachine::HandleReelStopped);
	}
}

bool ASlotMachine::TryGetWinningRule(FSlotPayoutRule& OutRule) const
{
	if (Reels.Num() < 3 || !Reels[0] || !Reels[1] || !Reels[2])
	{
		return false;
	}

	const FName A = Reels[0]->GetFinalSymbol();
	const FName B = Reels[1]->GetFinalSymbol();
	const FName C = Reels[2]->GetFinalSymbol();

	if (A.IsNone() || B.IsNone() || C.IsNone())
	{
		return false;
	}

	for (const FSlotPayoutRule& Rule : PayoutRules)
	{
		if (Rule.MatchCount != 3)
		{
			continue;
		}

		if (A == Rule.SymbolID && B == Rule.SymbolID && C == Rule.SymbolID)
		{
			OutRule = Rule;
			return true;
		}
	}

	return false;
}

void ASlotMachine::HandleReelStopped(ASlotReel* Reel)
{
	if (!bSpinResolved && AreAllReelsStopped())
	{
		bSpinResolved = true;
		ResolveSpin();
	}
}

void ASlotMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Reels.Num() < 3 || !Reels[0] || !Reels[1] || !Reels[2])
	{
		GEngine->AddOnScreenDebugMessage(9999, 0.0f, FColor::Red, TEXT("Reels not set / null"));
		return;
	}

	const float A = Reels[0]->GetCurrentSpinAngle();
	const float B = Reels[1]->GetCurrentSpinAngle();
	const float C = Reels[2]->GetCurrentSpinAngle();
}

void ASlotMachine::ApplyCurrentReelSpeed()
{
	for (ASlotReel* Reel : Reels)
	{
		if (Reel)
		{
			Reel->SetSpinIntervalScale(CurrentSpinIntervalScale);
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Cyan,
			FString::Printf(TEXT("Applied Reel Speed Scale: %.2f"), CurrentSpinIntervalScale)
		);
	}
}

void ASlotMachine::StartMachine()
{
	ApplyCurrentReelSpeed();
	for (auto* Reel : Reels)
	{
		if (Reel)
		{
			Reel->StartSpinning();
		}
	}
}

void ASlotMachine::StopReel(int32 ReelIndex)
{
	if (!Reels.IsValidIndex(ReelIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("INVALID INDEX."));
		return;
	}

	if (!Reels[ReelIndex]->IsSpinning())
		return;

	Reels[ReelIndex]->StopSpinning();
}

bool ASlotMachine::AreAllReelsStopped() const
{
	for (const auto* Reel : Reels)
	{
		if (Reel && Reel->IsSpinning())
		{
			return false;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("All Reels Stopped."));
	return true;
}

void ASlotMachine::ResolveSpin()
{
	const FName A = Reels[0]->GetFinalSymbol();
	const FName B = Reels[1]->GetFinalSymbol();
	const FName C = Reels[2]->GetFinalSymbol();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::White,
			FString::Printf(TEXT("Result: %s | %s | %s"), *A.ToString(), *B.ToString(), *C.ToString())
		);
	}

	FSlotPayoutRule WinningRule;
	if (TryGetWinningRule(WinningRule))
	{
		HandleWin(WinningRule);
	}
	else
	{
		HandleLoss();
	}
}

bool ASlotMachine::IsWinningSpin() const
{
	FSlotPayoutRule DummyRule;
	return TryGetWinningRule(DummyRule);
}

void ASlotMachine::HandleWin(const FSlotPayoutRule& WinningRule)
{
	CurrentSpinIntervalScale = FMath::Clamp(
		CurrentSpinIntervalScale - WinIntervalScaleStep,
		MinSpinIntervalScale,
		MaxSpinIntervalScale
	);

	PayOut(WinningRule.PayoutTierIndex);

	const FString Message = WinningRule.WinMessage.IsEmpty()
		? FString::Printf(TEXT("WIN: %s x%d"), *WinningRule.SymbolID.ToString(), WinningRule.MatchCount)
		: WinningRule.WinMessage.ToString();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Green, Message);
	}

	ScheduleRestart();
}

void ASlotMachine::HandleLoss()
{
	CurrentSpinIntervalScale = FMath::Clamp(
		CurrentSpinIntervalScale + LossIntervalScaleStep,
		MinSpinIntervalScale,
		MaxSpinIntervalScale
	);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("LOOOOOSERRRR"));

	ScheduleRestart();
}

void ASlotMachine::PayOut(int32 TierIndex)
{
	if (!PayoutTiers.IsValidIndex(TierIndex)) return;
	if (!PayoutTokens.IsValidIndex(TierIndex)) return;

	int32 Total = PayoutTiers[TierIndex];
	int32 Tokens = PayoutTokens[TierIndex];
	
	if (Tokens <= 0)
	{
		return;
	}

	PayoutValuePerChip = Total / Tokens;
	RemainingPayout = Tokens;

	GetWorld()->GetTimerManager().ClearTimer(PayoutTimer);

	SpawnNextPayoutChip();

	if (RemainingPayout > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			PayoutTimer,
			this,
			&ASlotMachine::SpawnNextPayoutChip,
			PayoutInvterval,
			true
		);
	}
}

void ASlotMachine::PrintReelSymbolData() const
{
	if (!GEngine) return;

	const float Duration = 12.0f;

	int32 Key = 1010; 

	GEngine->AddOnScreenDebugMessage(Key++, Duration, FColor::White,
		FString::Printf(TEXT("PrintReelSymbolData: Reels.Num=%d"), Reels.Num()));

	for (int32 ReelIdx = 0; ReelIdx < Reels.Num(); ++ReelIdx)
	{
		const ASlotReel* Reel = Reels[ReelIdx];
		if (!Reel)
		{
			GEngine->AddOnScreenDebugMessage(Key++, Duration, FColor::Red,
				FString::Printf(TEXT("Reel[%d] = NULL"), ReelIdx));
			continue;
		}

		GEngine->AddOnScreenDebugMessage(Key++, Duration, FColor::White,
			FString::Printf(TEXT("=== Reel[%d] %s ==="), ReelIdx, *Reel->GetName()));

		const TArray<FSlotSymbol>& Symbols = Reel->Symbols;

		if (Symbols.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(Key++, Duration, FColor::Yellow,
				TEXT("  (No symbols)"));
			continue;
		}

		for (int32 SymIdx = 0; SymIdx < Symbols.Num(); ++SymIdx)
		{
			const FSlotSymbol& S = Symbols[SymIdx];

			GEngine->AddOnScreenDebugMessage(Key++, Duration, FColor::Cyan,
				FString::Printf(TEXT("  [%d] ID=%s  Angle=%.2f  Weight=%.2f"),
					SymIdx,
					*S.SymbolID.ToString(),
					S.Angle,
					S.Weight
				));
		}
	}
}

void ASlotMachine::ScheduleRestart()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(RestartTimer))
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		RestartTimer,
		this,
		&ASlotMachine::RestartMachine,
		RestartDelay,
		false
	);
}

void ASlotMachine::RestartMachine()
{
	bSpinResolved = false;

	bAcceptingButtons = false;

	FTimerHandle ReArmButtons;
	GetWorld()->GetTimerManager().SetTimer(
		ReArmButtons,
		this,
		&ASlotMachine::AcceptButtonsNow,
		0.5f,
		false
	);
	
	OnSlotsStarted.Broadcast();

	StartMachine();
}

void ASlotMachine::AcceptButtonsNow()
{
	bAcceptingButtons = true;
}

void ASlotMachine::SpawnNextPayoutChip()
{
	if (RemainingPayout <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(PayoutTimer);
		return;
	}

	ASlotChip* Chip = GetWorld()->SpawnActor<ASlotChip>(SlotChipsBP, ChipSpawnPoint->GetComponentTransform());

	if (Chip)
		Chip->SetChipValue(PayoutValuePerChip);

	RemainingPayout--;

	if (RemainingPayout <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(PayoutTimer);
	}
}



