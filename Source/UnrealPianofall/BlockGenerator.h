// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <array>
#include <queue>
#include <fstream>
#include "BlockGenerator.generated.h"

UCLASS()
class UNREALPIANOFALL_API ABlockGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlockGenerator();

	FString midi_fileName;
	FString screenshot_path_savedir = "";
	uint32 blocklimit = 6000;
	uint16 spawnreduction = 4;
	bool highres = false;
	float capture_resolution = 1.0f;
	FVector2D ViewportSize;
	uint32 screenshot_x;
	uint32 screenshot_y;
	uint32 startframe = 0;
	float block_x = 0.0f;
	float block_y = 0.0;
	float block_z = 5000.0f;
	FVector blockscale = FVector(1.0f);
	float spawndist_x = 100.0f;
	float spawndist_y = 100.0f;
	bool midi_out_enabled = true;
	bool midi_out_off_enabled = false;
	bool capture_enabled = false;
	bool wait_for_load = true;
	uint64 PPQ_overwrite = 0;
	//Note: Default value sould be equal for the CameraManager
	uint32 frames_wait_for_load = 60; //0;
	std::vector<std::array<uint32, 128>> spawnpos;
	std::vector<std::array<uint32, 128>> stoppos;
	std::queue<AActor*> blocks;
	FLinearColor rainbow[128];
	UMaterialInstanceDynamic* DynMaterial[128];
	//Signed so that it can be compared with (spawnpos.size() - 2) which can be below zero
	int64 FrameNr = 0;
	UStaticMesh* Block_Mesh;
	//UDestructibleMesh* Block_DM;
	UMaterial* Block_Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
