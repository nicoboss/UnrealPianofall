// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DestructibleMesh.h"
#include "Block.generated.h"

UCLASS()
class KITEDEMO_API ABlock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString name;
	int32 npos;
	int32 NoteNr;
	UMaterialInstanceDynamic* DynMaterial;
	USceneComponent* BlockRoot;
	UStaticMeshComponent* MeshComp;
	//UDestructibleComponent* MeshComp_DM;
};
