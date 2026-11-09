// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotReel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReelStopped, ASlotReel*, Reel);

USTRUCT(BlueprintType)
struct FSlotSymbol
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Reel Props")
	FName SymbolID;

	UPROPERTY(EditAnywhere, Category = "Reel Props")
	float Weight = 1.f;

	UPROPERTY(VisibleAnywhere, Category = "Reel Props")
	float Angle = 0.f;
};

UCLASS()
class STICKYFINGERS_API ASlotReel : public AActor
{
	GENERATED_BODY()
	
public:
	ASlotReel();

	UPROPERTY(BlueprintAssignable, Category = "Reel")
	FOnReelStopped OnReelStopped;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartSpinning();

	UFUNCTION(BlueprintCallable)
	void StopSpinning();

	UFUNCTION(BlueprintCallable)
	bool IsSpinning() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentSpinAngle() const;

	UFUNCTION(BlueprintCallable)
	FName GetFinalSymbol() const;

	UFUNCTION(BlueprintCallable)
	void RebuildSymbolAnglesEven();

protected:
	int32 ChooseWeightedSymbolIndex() const;
	float GetAngleForIndex(int32 Index) const;
	void ApplyCurrentAngle();
	float GetCurrentStepInterval() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reel")
	UStaticMeshComponent* ReelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reel | Symbols")
	TArray<FSlotSymbol> Symbols;

	UPROPERTY(EditAnywhere, Category = "Reel | Symbols")
	float StartAngleOffset = 0.f;

	UPROPERTY(EditAnywhere, Category = "Reel | Spin")
	float FastStepInterval = 0.06f;

	UPROPERTY(EditAnywhere, Category = "Reel | Spin")
	float SlowStepInterval = 0.22f;

	UPROPERTY(EditAnywhere, Category = "Reel | Spin")
	int32 ExtraStepsOnStop = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reel|Speed")
	float SpinIntervalScale = 1.0f;

	UFUNCTION(BlueprintCallable)
	void SetSpinIntervalScale(float NewScale);

	UFUNCTION(BlueprintCallable)
	float GetSpinIntervalScale() const { return SpinIntervalScale; }

private:
	bool bIsSpinning = false;
	bool bPendingStop = false;

	float StepTimer = 0.f;

	int32 CurrentIndex = 0;
	int32 PendingStopIndex = INDEX_NONE;
	int32 StepsRemainingBeforeStop = 0;
	int32 InitialStepsRemainingBeforeStop = 0;
};
