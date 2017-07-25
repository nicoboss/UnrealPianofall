// Fill out your copyright notice in the Description page of Project Settings.

#include "PianoActor.h"
#include "Engine/World.h"
#include "Block.h"

// Sets default values
APianoActor::APianoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APianoActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APianoActor::Tick(float DeltaTime)
{

	for (int i = 0; i < 100; ++i) {
		Super::Tick(DeltaTime);
		FVector location = FVector(FMath::RandRange(0.0f, 200.0f), FMath::RandRange(0.0f, 200.0f), 1000.0f);
		
		FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABlock>(ABlock::StaticClass(), location, rotate, SpawnInfo);
	}
}

