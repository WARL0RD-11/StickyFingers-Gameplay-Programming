// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotReel.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

ASlotReel::ASlotReel()
{
	PrimaryActorTick.bCanEverTick = true;

	ReelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReelMesh"));
	RootComponent = ReelMesh;
}

void ASlotReel::BeginPlay()
{
	Super::BeginPlay();

	RebuildSymbolAnglesEven();

	CurrentIndex = 0;
	ApplyCurrentAngle();
}

void ASlotReel::RebuildSymbolAnglesEven()
{
	const int32 N = Symbols.Num();
	if (N <= 0) return;

	const float Step = 360.f / float(N);

	for (int32 i = 0; i < N; ++i)
	{
		float Angle = StartAngleOffset + Step * i;
		Angle = FMath::Fmod(Angle, 360.f);
		if (Angle < 0.f)
		{
			Angle += 360.f;
		}

		Symbols[i].Angle = Angle;
	}
}

float ASlotReel::GetAngleForIndex(int32 Index) const
{
	if (!Symbols.IsValidIndex(Index))
	{
		return 0.f;
	}

	return Symbols[Index].Angle;
}

void ASlotReel::ApplyCurrentAngle()
{
	if (!ReelMesh || !Symbols.IsValidIndex(CurrentIndex))
	{
		return;
	}

	ReelMesh->SetRelativeRotation(FRotator(0.f, 0.f, Symbols[CurrentIndex].Angle));
}

float ASlotReel::GetCurrentStepInterval() const
{
	const float ScaledFast = FastStepInterval * SpinIntervalScale;
	const float ScaledSlow = SlowStepInterval * SpinIntervalScale;

	if (!bPendingStop || InitialStepsRemainingBeforeStop <= 0)
	{
		return ScaledFast;
	}

	const float Progress = 1.f - (float(StepsRemainingBeforeStop) / float(InitialStepsRemainingBeforeStop));
	return FMath::Lerp(ScaledFast, ScaledSlow, Progress);
}

void ASlotReel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsSpinning)
	{
		return;
	}

	StepTimer += DeltaTime;

	const float CurrentInterval = GetCurrentStepInterval();

	if (StepTimer >= CurrentInterval)
	{
		StepTimer -= CurrentInterval;

		if (Symbols.Num() > 0)
		{
			CurrentIndex = (CurrentIndex + 1) % Symbols.Num();
			ApplyCurrentAngle();
		}

		if (bPendingStop)
		{
			if (StepsRemainingBeforeStop > 0)
			{
				StepsRemainingBeforeStop--;
			}
			else if (CurrentIndex == PendingStopIndex)
			{
				bIsSpinning = false;
				bPendingStop = false;

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						2.0f,
						FColor::Green,
						FString::Printf(TEXT("Stopped on symbol: %s"), *GetFinalSymbol().ToString())
					);
				}
				OnReelStopped.Broadcast(this);
			}
		}
	}
}

void ASlotReel::SetSpinIntervalScale(float NewScale)
{
	SpinIntervalScale = FMath::Max(0.01f, NewScale);
}

void ASlotReel::StartSpinning()
{
	bIsSpinning = true;
	bPendingStop = false;
	StepTimer = 0.f;
	StepsRemainingBeforeStop = 0;
	InitialStepsRemainingBeforeStop = 0;
	PendingStopIndex = INDEX_NONE;
}

void ASlotReel::StopSpinning()
{
	if (Symbols.Num() == 0)
	{
		bIsSpinning = false;
		bPendingStop = false;
		return;
	}

	const int32 ChosenIndex = ChooseWeightedSymbolIndex();
	if (!Symbols.IsValidIndex(ChosenIndex))
	{
		bIsSpinning = false;
		bPendingStop = false;
		return;
	}

	PendingStopIndex = ChosenIndex;
	StepsRemainingBeforeStop = ExtraStepsOnStop;
	InitialStepsRemainingBeforeStop = ExtraStepsOnStop;
	bPendingStop = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Will stop on symbol: %s"), *Symbols[ChosenIndex].SymbolID.ToString())
		);
	}
}

bool ASlotReel::IsSpinning() const
{
	return bIsSpinning;
}

float ASlotReel::GetCurrentSpinAngle() const
{
	return GetAngleForIndex(CurrentIndex);
}

int32 ASlotReel::ChooseWeightedSymbolIndex() const
{
	if (Symbols.Num() == 0)
	{
		return INDEX_NONE;
	}

	float TotalWeight = 0.f;
	for (const FSlotSymbol& Symbol : Symbols)
	{
		TotalWeight += FMath::Max(0.f, Symbol.Weight);
	}

	if (TotalWeight <= 0.f)
	{
		return FMath::RandRange(0, Symbols.Num() - 1);
	}

	const float Roll = FMath::FRandRange(0.f, TotalWeight);
	float RunningWeight = 0.f;

	for (int32 i = 0; i < Symbols.Num(); ++i)
	{
		RunningWeight += FMath::Max(0.f, Symbols[i].Weight);

		if (Roll <= RunningWeight)
		{
			return i;
		}
	}

	return Symbols.Num() - 1;
}

FName ASlotReel::GetFinalSymbol() const
{
	if (!Symbols.IsValidIndex(CurrentIndex))
	{
		return NAME_None;
	}

	return Symbols[CurrentIndex].SymbolID;
}
