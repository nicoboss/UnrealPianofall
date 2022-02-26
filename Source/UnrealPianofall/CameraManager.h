/*
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

UCLASS()
class UNREALPIANOFALL_API ACameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraManager();

	uint8 scenenr = 1;
	float camerapos = 0;
	//Note: Default value sould be equal for the BlockGenerator
	uint32 frames_wait_for_camera_fix = 0;
	float camerapos_first_scene = 180.0f; //120.0f;
	bool camera_fix = false;

	//Starting here with 1 because the increasement is at the end
	uint32 camera_fix_framecount = 1;

	FVector camera_fix_vector;
	FRotator camera_fix_rotator;
	float camera_speed = 1.0f;
	bool camera_no_repeat = false;
	FVector centerpos = FVector(0.0f, 0.0f, 20000.0f);
	FVector centerposmid = FVector(0.0f, 0.0f, 10000.0f);
	FVector centerposdown = FVector(0.0f, 0.0f, 2000.0f);
	FVector playerstartpos = FVector(102225.0f, 647403.0f, -113628.0f);
	FVector playerlastpos = FVector(102225.0f, 647403.0f, -113628.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

*/