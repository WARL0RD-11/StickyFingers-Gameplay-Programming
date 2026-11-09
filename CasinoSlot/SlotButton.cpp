// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotButton.h"
#include "Components/BoxComponent.h"
#include "SlotMachine.h"

// Sets default values
ASlotButton::ASlotButton()
{
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	RootComponent = ButtonMesh;

	ButtonTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	ButtonTrigger->SetupAttachment(RootComponent);
}

void ASlotButton::ResetButtonState()
{
	bHasLeft = false;
	bHasRight = false;
	bHasBoth = false;
}

void ASlotButton::BeginPlay()
{
	Super::BeginPlay();
	ButtonTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASlotButton::OnOverlap);
	ButtonTrigger->OnComponentEndOverlap.AddDynamic(this, &ASlotButton::OnOverLapEnd);
}

void ASlotButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlotButton::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!mSlotMachine) return;

	if (OtherComp->ComponentHasTag("RightHand"))
		bHasRight = true;

	if (OtherComp->ComponentHasTag("LeftHand"))
		bHasLeft = true;

	bHasBoth = bHasLeft && bHasRight;
	bool bValid = false;

	switch (HandRequirement)
	{
	case EButtonHandNeeded::Right :
			bValid = bHasRight;
			ReelIndex = 0;
			break;
	case EButtonHandNeeded::Left:
		bValid = bHasLeft;
		ReelIndex = 2;
		break;
	case EButtonHandNeeded::Both:
		bValid = bHasBoth;
		ReelIndex = 1;
		break;
	}

	if (bValid && mSlotMachine->IsAcceptingButton())
	{
		mSlotMachine->StopReel(ReelIndex);
	}
}

void ASlotButton::OnOverLapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherComp->ComponentHasTag("RightHand"))
		bHasRight = false;

	if (OtherComp->ComponentHasTag("LeftHand"))
		bHasLeft = false;

	if(bHasLeft == false && bHasRight == false )
		bHasBoth = false;
}
