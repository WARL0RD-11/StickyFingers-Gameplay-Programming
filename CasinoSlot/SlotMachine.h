// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "SlotMachine.generated.h"

class ASlotReel;
class ASlotChip;

USTRUCT(BlueprintType)
struct FSlotPayoutRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    FName SymbolID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    int32 MatchCount = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    int32 PayoutTierIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    FText WinMessage;
};

UCLASS()
class STICKYFINGERS_API ASlotMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	ASlotMachine();
	virtual void Tick(float DeltaTime) override;

	void StartMachine();
	void StopReel(int32 ReelIndex);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ASlotReel*> Reels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    TArray<int32> PayoutTiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    TArray<int32> PayoutTokens;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Machine|Tolerance")
    float CurrentSpinIntervalScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Machine|Tolerance")
    float LossIntervalScaleStep = 0.10f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Machine|Tolerance")
    float WinIntervalScaleStep = 0.08f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Machine|Tolerance")
    float MinSpinIntervalScale = 0.50f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Machine|Tolerance")
    float MaxSpinIntervalScale = 2.00f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ASlotChip> SlotChipsBP;

    UPROPERTY(EditAnywhere)
    TObjectPtr<USceneComponent> ChipSpawnPoint;

    UPROPERTY(EditAnywhere, Category = "Slot Machine")
    float RestartDelay = 1.5f;

    bool IsAcceptingButton() const { return bAcceptingButtons; }

    UPROPERTY(EditAnywhere, Category = "Payout")
    float PayoutInvterval = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payout")
    TArray<FSlotPayoutRule> PayoutRules;

    UFUNCTION(BlueprintCallable)
    void ApplyCurrentReelSpeed();

    UFUNCTION()
    void HandleReelStopped(ASlotReel* Reel);

    UFUNCTION(BlueprintCallable)
    void BindReels();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotsStarted);
	
	UPROPERTY(BlueprintAssignable);
	FOnSlotsStarted OnSlotsStarted;

protected:
	virtual void BeginPlay() override;
    bool TryGetWinningRule(FSlotPayoutRule& OutRule) const;

private:	
    bool AreAllReelsStopped() const;
    void ResolveSpin();

    bool IsWinningSpin() const;
    void HandleWin(const FSlotPayoutRule& WinningRule);
    void HandleLoss();
    void PayOut(int32 TierIndex);

    UFUNCTION(BlueprintCallable)
    void PrintReelSymbolData() const;

    float CurrentTolerance = 0.f;

    //--------------RESTARTING MODIFIERS-------------------
    FTimerHandle RestartTimer;
    bool bSpinResolved = false;
    void ScheduleRestart();
    void RestartMachine();

    bool bAcceptingButtons = false;
    void AcceptButtonsNow();

    //--------------PAYOUT MODIFIERS-------------------
    FTimerHandle PayoutTimer;
    int32 RemainingPayout = 0;
    int32 PayoutValuePerChip = 0;
    void SpawnNextPayoutChip();


};
