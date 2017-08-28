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

	std::ifstream midifile;
	uint16 spawnreduction = 5;
	uint32 blocklimit = 6000;
	std::vector<std::array<uint32, 128>> spawnpos;
	std::vector<std::array<uint32, 128>> stoppos;
	std::queue<AActor*> blocks;
	FLinearColor rainbow[128];
	uint32 FrameNr = 0;
	UStaticMesh* Block_Mesh;
	UMaterial* Block_Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
