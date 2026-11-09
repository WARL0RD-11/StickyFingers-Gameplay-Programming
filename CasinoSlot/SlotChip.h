// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotChip.generated.h"

UCLASS()
class STICKYFINGERS_API ASlotChip : public AActor
{
	GENERATED_BODY()
	
public:	
	ASlotChip();
	virtual void Tick(float DeltaTime) override;

	void SetChipValue(int32 NewValue);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ChipMesh;	

private:	
	int32 ChipValue;

};
