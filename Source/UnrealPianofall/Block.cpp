// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "Engine.h"
#include "Components/BoxComponent.h"



// Sets default values
ABlock::ABlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("/Engine/EditorMeshes/EditorCube.EditorCube"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/StarterContent/Materials/M_Metal_Burnished_Steel"));



	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetSimulatePhysics(true);

	//if (FoundMesh.Succeeded())
	MeshComp->SetStaticMesh(FoundMesh.Object);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Hi");
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	//TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("VolumeTrigger"));
	//TriggerVolume->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	//TriggerVolume->SetupAttachment(RootComponent);
	//TriggerVolume->SetVisibility(true);
	//TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//if (FoundMaterial.Succeeded())
	StoredMaterial = FoundMaterial.Object;
	MeshComp->SetMaterial(0, StoredMaterial);


}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


