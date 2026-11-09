// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Engine/World.h"
#include "PoolInterface.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	if(bWarmPoolOnBeginPlay)
	{
		WarmPool();
	}

	if(bSpawnOnBeginPlay)
	{
		SpawnNowPooled();
	}	
}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyPool();
	Super::EndPlay(EndPlayReason);
}


void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::WarmPool()
{
	if(!GetWorld() || !ActorToPool)
	{
		return;
	}

	const int32 NeededToSpawn = FMath::Max(0,InitPoolSize - InactivePool.Num() - ActiveActors.Num());

	for(int32 i = 0; i < NeededToSpawn; ++i)
	{
		AActor* NewActor = CreatePooledActor();
		if(NewActor)
		{
			InactivePool.Add(NewActor);
		}
	}
}


AActor* ASpawner::CreatePooledActor()
{
	if(!GetWorld() || !ActorToPool)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = CollisionHandling;

	TObjectPtr<AActor> NewActor = GetWorld()->SpawnActor<AActor>(ActorToPool, GetActorLocation(), GetActorRotation(), SpawnParams);

	if(!NewActor)
	{
		return nullptr;
	}

	DeactivateActor(NewActor);
	return NewActor;
}

AActor* ASpawner::SpawnOnePooled()
{
	if (!GetWorld() || !ActorToPool)
	{
		return nullptr;
	}

	TObjectPtr<AActor> PickedActor = nullptr;
	if(InactivePool.Num() > 0)
	{
		PickedActor = InactivePool.Pop(EAllowShrinking::No);
	}
	else 
	{
		if (bRecycleOldestWhenEmpty && ActiveActors.Num() > 0)
		{
			PickedActor = ActiveActors[0];
			DespawnActor(PickedActor);

			if(InactivePool.Num() > 0)
			{
				PickedActor = InactivePool.Pop(EAllowShrinking::No);
			}
		}
		else if (bShouldExpandPool)
		{
			PickedActor = CreatePooledActor();

		}
	}

	if(!PickedActor)
	{
		return nullptr;
	}

	const FVector SpawnLocation = GetSpawnLocation();
	const FRotator SpawnRotation = GetSpawnRotation();
	ActivateActor(PickedActor, SpawnLocation, SpawnRotation);
	ActiveActors.Add(PickedActor);

	if(PickedActor->GetClass()->ImplementsInterface(UPoolInterface::StaticClass()))
	{
		IPoolableActor::Execute_OnPooledSpawned(PickedActor);
	}
	return PickedActor;
}

void ASpawner::SpawnNowPooled()
{
	const int32 CountToSpawn = SpawnCountMode == ESpawnCount::Single ? 1 : FMath::Max(1,SpawnCount);
	for(int32 i = 0; i < CountToSpawn; ++i)
	{
		SpawnOnePooled();
	}	
}

void ASpawner::DespawnActor(AActor* ActorToDespawn)
{
	if (!ActorToDespawn)
		return;
	
	const int32 Index = ActiveActors.Find(ActorToDespawn);

	if (Index == INDEX_NONE)
		return;

	if (ActorToDespawn->GetClass()->ImplementsInterface(UPoolInterface::StaticClass()))
	{
		IPoolableActor::Execute_OnPooledDespawned(ActorToDespawn);
	}

	DeactivateActor(ActorToDespawn);
	InactivePool.Add(ActorToDespawn);
}

void ASpawner::DestroyPool()
{
	for(AActor* A : ActiveActors)
	{
		if(A)
		{
			A->Destroy();
		}
	}

	for(AActor* A : InactivePool)
	{
		if(A)
		{
			A->Destroy();
		}
	}

	ActiveActors.Reset();
	InactivePool.Reset();
}

FVector ASpawner::GetSpawnLocation() const
{
	if (SpawnLocationMode == ESpawnLocationMode::Fixed)
	{
		return GetActorLocation();
	}

	const FVector Origin = GetActorLocation();

	const float X = FMath::FRandRange(-SpawnVolumeExtent.X, SpawnVolumeExtent.X);
	const float Y = FMath::FRandRange(-SpawnVolumeExtent.Y, SpawnVolumeExtent.Y);

	const FVector LocalOffset(X, Y, 0.0f);
	const FVector WorldOffset = GetActorRotation().RotateVector(LocalOffset);

	return Origin + WorldOffset;
}

FRotator ASpawner::GetSpawnRotation() const
{
	FRotator R = GetActorRotation();
	if (bRandomizeSpawnRotation)
	{
		R.Yaw = FMath::FRandRange(0.f, 360.f);
	}
	return R;
}

void ASpawner::ActivateActor(AActor* ActorToActivate, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!ActorToActivate) return;

	ActorToActivate->SetActorLocationAndRotation(SpawnLocation, SpawnRotation, false, nullptr, ETeleportType::TeleportPhysics);

	ActorToActivate->SetActorHiddenInGame(false);
	ActorToActivate->SetActorEnableCollision(true);
	ActorToActivate->SetActorTickEnabled(true);
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ActorToActivate->GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(true);
	}
}

void ASpawner::DeactivateActor(AActor* ActorToDeactivate)
{
	if (!ActorToDeactivate) return;

	ActorToDeactivate->SetActorHiddenInGame(true);
	ActorToDeactivate->SetActorEnableCollision(false);
	ActorToDeactivate->SetActorTickEnabled(false);
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ActorToDeactivate->GetRootComponent()))
	{
		PrimComp->SetSimulatePhysics(false);
	}
}