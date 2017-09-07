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
class KITEDEMO_API ABlockGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlockGenerator();

	FString midi_fileName;
	FString screenshot_path_savedir = "";
	uint32 blocklimit = 6000;
	uint16 spawnreduction = 4;
	uint16 startframe = 0;
	float block_x = 90600.0f;
	float block_y = 645000.0;
	float block_z = -110000.0f;
	FVector blockscale = FVector(1.0f);
	float spawndist_x = 100.0f;
	float spawndist_y = 100.0f;
	bool midi_out_enabled = false;
	bool midi_out_off_enabled = false;
	bool capture_enabled = false;
	bool wait_for_load = true;
	uint64 PPQ_overwrite = 0;
	//Note: Default value sould be equal for the CameraManager
	uint16 frames_wait_for_load = 420; //0;
	std::vector<std::array<uint32, 128>> spawnpos;
	std::vector<std::array<uint32, 128>> stoppos;
	std::queue<AActor*> blocks;
	FLinearColor rainbow[128];
	uint32 FrameNr = 0;
	UStaticMesh* Block_Mesh;
	//UDestructibleMesh* Block_DM;
	UMaterial* Block_Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
