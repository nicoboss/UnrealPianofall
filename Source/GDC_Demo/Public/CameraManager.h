// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

UCLASS()
class KITEDEMO_API ACameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraManager();

	uint8 scenenr = 0;
	float framenr = 0.0;
	FVector centerpos = FVector(90600.0f, 645000.0f - 6400.0f, -110000.0f);
	FVector centerposdown = FVector(90600.0f, 645000.0f - 6400.0f, -114000.0f);
	FVector playerstartpos = FVector(102225.0f, 647403.0f, -113628.0f);
	FVector playerlastpos = FVector(102225.0f, 647403.0f, -113628.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
