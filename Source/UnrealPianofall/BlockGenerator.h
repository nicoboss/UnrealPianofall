// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <array>
#include "BlockGenerator.generated.h"

UCLASS()
class UNREALPIANOFALL_API ABlockGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlockGenerator();

	std::vector<std::array<uint32, 128>> spawnpos;
	FLinearColor rainbow[128];
	uint32 FrameNr;
	UStaticMesh* Block_Mesh;
	UMaterial* Block_Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
