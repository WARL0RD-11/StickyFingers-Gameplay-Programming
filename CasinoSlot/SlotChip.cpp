// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotChip.h"

ASlotChip::ASlotChip()
{
	PrimaryActorTick.bCanEverTick = true;

	ChipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChipMesh"));
	RootComponent = ChipMesh;	
}

void ASlotChip::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlotChip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlotChip::SetChipValue(int32 NewValue)
{
	ChipValue = NewValue;
}

