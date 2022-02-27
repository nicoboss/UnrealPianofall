// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "BlockGenerator.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include <math.h>
 
// Sets default values§
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	//Only defined AFTER the ABlock::ABlock() constructor!
	ABlockGenerator* AOwner = (ABlockGenerator*)this->GetOwner();

	FVector loc = this->GetActorLocation();
	float newloc_z = floor(loc.Z);
	NoteNr = (int32)((loc.Z - newloc_z) * 1000.0);
	//UE_LOG(LogTemp, Log, TEXT("loc.Z %f => %u   newloc_z: %f"), loc.Z, NoteNr, newloc_z);
	loc.Z = newloc_z;
	this->SetActorLocation(loc);

	DynMaterial = UMaterialInstanceDynamic::Create(AOwner->Block_Material, NULL);
	DynMaterial->SetVectorParameterValue(FName("Color"), AOwner->rainbow[NoteNr]);

	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetStaticMesh(AOwner->Block_Mesh);
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComp->SetMaterial(0, DynMaterial);
}

void ABlock::RefreshColor()
{
	//Only defined AFTER the ABlock::ABlock() constructor!
	ABlockGenerator* AOwner = (ABlockGenerator*)this->GetOwner();

	FVector loc = this->GetActorLocation();
	float newloc_z = floor(loc.Z);
	NoteNr = (int32)((loc.Z - newloc_z) * 1000.0);
	//UE_LOG(LogTemp, Log, TEXT("loc.Z %f => %u   newloc_z: %f"), loc.Z, NoteNr, newloc_z);
	loc.Z = newloc_z;
	this->SetActorLocation(loc);

	DynMaterial->SetVectorParameterValue(FName("Color"), AOwner->rainbow[NoteNr]);
}

// Called every frame
// Is by default disabled in ABlock::ABlock()
// The BlockGebnerator handels despawning using a FIFO-List
// which is much more efficient instead calling each block every frame.
// Expect a huge performance impact if you ever need to enable this.
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
