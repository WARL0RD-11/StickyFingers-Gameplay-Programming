// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "PoolInterface.generated.h"

/**
 * 
 */

UINTERFACE(BlueprintType)
class STICKYFINGERS_API UPoolableActor : public UInterface
{
	GENERATED_BODY()
};

class STICKYFINGERS_API IPoolableActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnPooledSpawned();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnPooledDespawned();
};

UCLASS()
class STICKYFINGERS_API UPoolInterface : public UObject
{
	GENERATED_BODY() 
};
