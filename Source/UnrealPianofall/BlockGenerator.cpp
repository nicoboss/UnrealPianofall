// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockGenerator.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include "Engine.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Public/ImageUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Block.h"

//int viewStats = 0;

// Sets default values
ABlockGenerator::ABlockGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("/Engine/EditorMeshes/EditorCube.EditorCube"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/UnrealPianofall/Materials/M_Advanced_Block"));

	Block_Mesh = FoundMesh.Object;
	Block_Material = FoundMaterial.Object;

	FrameNr = 0;

	for (int i = 0; i < 22; ++i) { //green up
		rainbow[i] = { 255.0f, i*0.04545454545f, 0.0f };
	}
	for (int i = 22; i < 43; ++i) { //red down
		rainbow[i] = { 1.0f - ((i - 22)*0.04761904761f), 255.0f, 0.0f };
	}
	for (int i = 43; i < 64; ++i) { //blue up
		rainbow[i] = { 0.0f, 1.0f, (i - 43)*0.04761904761f };
	}
	for (int i = 64; i < 85; ++i) { //green down
		rainbow[i] = { 0.0f, 1.0f - ((i - 64)*0.04761904761f), 255.0f };
	}
	for (int i = 85; i < 106; ++i) { //reed up
		rainbow[i] = { (i - 85)*0.04761904761f, 0.0f, 255.0f };
	}
	for (int i = 106; i < 128; ++i) { //blue down
		rainbow[i] = { 255.0f, 0.0f, 1.0f - ((i - 106)*0.04545454545f) };
	}

}

// Called when the game starts or when spawned
void ABlockGenerator::BeginPlay()
{
	Super::BeginPlay();

	std::ifstream midifile("A:\\Music\\britney_spears-born_to_make_you_happy.mid", std::ios::binary);

	std::vector<std::vector<uint32>> vecArray;

	char header[14];
	midifile.read(header, 14);
	if (header[0] == 'M' &&
		header[1] == 'T' &&
		header[2] == 'h' &&
		header[3] == 'd' &&
		header[4] == 0 &&
		header[5] == 0 &&
		header[6] == 0 &&
		header[7] == 6) {
		UE_LOG(LogTemp, Log, TEXT("Valid MIDI-file!"));
		uint16 PPQ = (header[12] << 8) | header[13];
		if (PPQ == 0) {
			PPQ = 120;
		}
		UE_LOG(LogTemp, Log, TEXT("PPQ: %u"), PPQ);
		uint32 usPQ = 500000; //120 PBM
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MIDI-File: Invalid header"));
		UE_LOG(LogTemp, Error, TEXT("Invalid MIDI-file!"));
	}

	bool in_MTrk = false;
	char MTrk[4] = {'M','T','r','k'};
	uint8 MTrk_findpos = 0;
	uint16 MTrk_readpos = 0;
	int32 MTrk_nr = -1;
	char buf[4096];
	do {
		UE_LOG(LogTemp, Log, TEXT("Read Block"));
		midifile.read(buf, 4096);

		MTrk_readpos = 0;
		if (in_MTrk == false) {
			for (uint16 i = 0; i < 4096; ++i) {
				if (buf[i] == MTrk[MTrk_findpos]) {
					UE_LOG(LogTemp, Log, TEXT("MTrk_findpos: %u"), MTrk_findpos + 1);
					if (++MTrk_findpos == 4) {
						UE_LOG(LogTemp, Log, TEXT("Found!"));
						MTrk_findpos = 0;
						++MTrk_nr;
						MTrk_readpos = i;
						in_MTrk = true;
						break;
					}
				} else {
					if (buf[i] == MTrk[0]) {
						MTrk_findpos = 1;
					} else {
						MTrk_findpos = 0;
					}
				}
			}
		}

		if (in_MTrk == true) {
			for (uint16 i = MTrk_readpos; i < 4096; ++i) {
				//UE_LOG(LogTemp, Log, TEXT("Hello World!"));
			}
		}
	} while (midifile);

}

// Called every frame
void ABlockGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	++FrameNr;

	int32 notenr_rnd;

	//for (int i = 0; i < 100; ++i) {
	notenr_rnd = (uint8)FMath::RandRange(0.0f, 127.0f);
	FVector location = FVector((float)(6400 - notenr_rnd * 100), 0.0f, 4000.0f);
	FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.Owner = this;
	SpawnInfo.Name = *FString::Printf(TEXT("F%uN%u"), FrameNr, notenr_rnd);
	AActor* newBlock = GetWorld()->SpawnActor<ABlock>(ABlock::StaticClass(), location, rotate, SpawnInfo);
	//}


	//++viewStats;
	//if (viewStats == 10) {
	//viewStats = 0;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlock::StaticClass(), FoundActors);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, "FPS: " + FString::SanitizeFloat(1.0f / DeltaTime) + "\n" + "Blocks: " + FString::FromInt(FoundActors.Num()));
	//}

	//GetHighResScreenshotConfig().ResolutionMultiplier = 4; //Sets the res multiplier
	//GetWorld()->GetGameViewport()->Viewport->TakeHighResScreenShot(); //Sets the flag in the viewport to take the high-res shot.

}
