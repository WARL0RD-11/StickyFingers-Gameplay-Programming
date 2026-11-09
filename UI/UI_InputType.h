// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_InputType.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EUIInputDevice : uint8
{
	Keyboard		UMETA(DisplayName = "KeyboardMouse"),
	Xbox			UMETA(DisplayName = "Xbox"),
	PlayStation		UMETA(DisplayName = "PlayStation"),
	NintendoSwitch	UMETA(DisplayName = "NintendoSwitch"),
	Unknown			UMETA(DisplayName = "Unknown")
};

USTRUCT(BlueprintType)
struct FUIActionPrompt
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SelectText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText BackText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PauseText;
};