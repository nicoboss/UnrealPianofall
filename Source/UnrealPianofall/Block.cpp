// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "BlockGenerator.h"
#include "Engine.h"
#include "Components/BoxComponent.h"



// Sets default values
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
	name = this->GetName();
	name.FindChar('N', npos);
	NoteNr = FCString::Atoi(*name.RightChop(npos + 1));
	UE_LOG(LogTemp, Log, TEXT("%d"), NoteNr);

	DynMaterial = UMaterialInstanceDynamic::Create(AOwner->Block_Material, NULL);
	DynMaterial->SetVectorParameterValue(FName("Color"), AOwner->rainbow[NoteNr]);
	//DynMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(1.0f, 0.8f, 0.8f));
	//if (FMath::RandBool() == true) {
	//	DynMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(100.0f, 0.0f, 0.0f));
	//}
	//else {
	//	DynMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(100.0f, 100.0f, 0.0f));
	//}

	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetStaticMesh(AOwner->Block_Mesh);
	MeshComp->SetMaterial(0, DynMaterial);

}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


