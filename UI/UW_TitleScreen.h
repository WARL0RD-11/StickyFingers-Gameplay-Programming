// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_BaseWidget.h"
#include "UW_TitleScreen.generated.h"

//Forward Declarations
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class STICKYFINGERS_API UUW_TitleScreen : public UUW_BaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UPROPERTY(EditAnywhere,meta = (BindWidget))
	TObjectPtr<UTextBlock> FlashText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Background")
	TObjectPtr<UTexture2D> BackgroundTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (ClampMin = "0.01"))
	float FlashSpeed = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MinAlpha = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MaxAlpha = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash")
	FLinearColor BaseColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulse", meta = (ClampMin = "0.0"))
	float MinScale = 0.95f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulse", meta = (ClampMin = "0.0"))
	float MaxScale = 1.05f;

	/** Scale pulse speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pulse", meta = (ClampMin = "0.01"))
	float ScaleSpeed = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUW_MainM> pMainMenu;

	virtual void RefreshUI() override;

private: 
	float ElapsedTime = 0.0f;
	float ComputeAplha() const;
	float ComputeScale(float Time) const;
	APlayerController* PC = nullptr;	
};
