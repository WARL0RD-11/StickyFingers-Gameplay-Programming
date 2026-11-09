// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UENUM(BlueprintType)
enum class ESpawnLocationMode : uint8
{
	Fixed			UMETA(DisplayName = "Fixed (Spawner Transform)"),
	RandomInVolume	UMETA(DisplayName = "Random in Volume")
};

UENUM(BlueprintType)
enum class ESpawnCount : uint8
{
	Single		UMETA(DisplayName = "Single Instance"),
	Multiple	UMETA(DisplayName = "Multple Instances")
};

UCLASS()
class STICKYFINGERS_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawner();
	virtual void Tick(float DeltaTime) override;

	//-----------Pool Functionality----------------

	UFUNCTION(BlueprintCallable, Category = "Spawning|Pool")
	AActor* SpawnOnePooled();

	UFUNCTION(BlueprintCallable, Category = "Spawning|Pool")
	void SpawnNowPooled();

	UFUNCTION(BlueprintCallable, Category = "Spawning|Pool")
	void DespawnActor(AActor* ActorToDespawn);

	UFUNCTION(BlueprintCallable, Category = "Spawning|Pool")
	void WarmPool();

	UFUNCTION(BlueprintCallable, Category = "Spawning|Pool")
	void DestroyPool();


	//-----------Settings for Pool----------------

	UPROPERTY(EditAnywhere, Category = "Spawning|Class")
	TSubclassOf<AActor> ActorToPool;

	UPROPERTY(EditAnywhere, Category = "Spawning|Pool")
	int32 InitPoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "Spawning|Pool")
	bool bShouldExpandPool = true;

	UPROPERTY(EditAnywhere, Category = "Spawning|Pool")
	bool bRecycleOldestWhenEmpty = false;

	//-----------Spawn Controls----------------

	UPROPERTY(EditAnywhere, Category = "Spawning|Location")
	ESpawnLocationMode SpawnLocationMode = ESpawnLocationMode::Fixed;

	UPROPERTY(EditAnywhere, Category = "Spawning|Location", meta = (EditCondition = "LocationMode==ESpawnLocationMode::RandomInBox"))
	FVector SpawnVolumeExtent = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditAnywhere, Category = "Spawning|Location")
	bool bRandomizeSpawnRotation = true;

	UPROPERTY(EditAnywhere, Category = "Spawning|Count")
	ESpawnCount SpawnCountMode = ESpawnCount::Single;

	UPROPERTY(EditAnywhere, Category = "Spawning|Count", meta = (EditCondition = "SpawnCountMode==ESpawnCount::Multiple", ClampMin = "1"))
	int32 SpawnCount = 5;

	UPROPERTY(EditAnywhere, Category = "Spawning|Timing")
	bool bWarmPoolOnBeginPlay = true;	

	UPROPERTY(EditAnywhere, Category = "Spawning|Timing")
	bool bSpawnOnBeginPlay = true;	


	UPROPERTY(EditAnywhere, Category = "Spawning|Collision")
	ESpawnActorCollisionHandlingMethod CollisionHandling = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	FVector GetSpawnLocation() const;	
	FRotator GetSpawnRotation() const;	

	AActor* CreatePooledActor();	
	void ActivateActor(AActor* ActorToActivate, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	void DeactivateActor(AActor* ActorToDeactivate);

	UPROPERTY()
	TArray<TObjectPtr<AActor>> InactivePool;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActiveActors;

};
