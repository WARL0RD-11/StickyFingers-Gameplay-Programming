// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotButton.generated.h"

class ASlotMachine;

UENUM(BlueprintType)
enum class EButtonHandNeeded : uint8
{
	Left,
	Right, 
	Both
};

UCLASS()
class STICKYFINGERS_API ASlotButton : public AActor
{
	GENERATED_BODY()
	
public:	
	ASlotButton();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 ReelIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EButtonHandNeeded HandRequirement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASlotMachine* mSlotMachine;

	void ResetButtonState();
protected:
	virtual void BeginPlay() override;

private:	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ButtonMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> ButtonTrigger;

	bool bHasRight = false;
	bool bHasLeft = false;
	bool bHasBoth = false;

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverLapEnd(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};
