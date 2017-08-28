// Fill out your copyright notice in the Description page of Project Settings.

#include "GDC_Demo.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include "Engine.h"
#include "CameraManager.h"


// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* world = GetWorld();
	APawn* PlayerPawn = world->GetFirstPlayerController()->GetPawn();
	framenr += 1.0;

	//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, "Scene: " + FString::FromInt(scenenr) + " framenr: " + FString::FromInt(framenr));

	switch (scenenr) {
	case 0: {
		if (framenr == 120.0) {
			framenr = 0.0;
			scenenr += 1;
		}
		break;
	}
	case 1: {
		FRotator rotate = FRotator((framenr / 10.0)-170, 0, 0);
		FVector NewLoc = centerpos + rotate.RotateVector(centerpos - playerstartpos);
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 1800.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 2: {
		FVector NewLoc = playerlastpos + (playerstartpos - playerlastpos) * framenr / 600.0f;
		FRotator NewRot = (centerpos - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 650.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 3: {
		FRotator rotate = FRotator(0, framenr / 4.0, 0);
		FVector NewLoc = centerpos + rotate.RotateVector(centerpos - playerstartpos);
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 1000.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 4: {
		FVector NewLoc = playerlastpos + (playerstartpos - playerlastpos) * framenr / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 600.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 5: {
		FVector Pos1 = FVector(38622.285, 703740.188, -107188.945);
		FVector Pos2 = FVector(40000.000, 703400.875, -107189.617);
		FVector NewLoc = Pos1 + (Pos2 - Pos1) * framenr / 60.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 60.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 6: {
		FVector Pos2 = FVector(59940.891, 680811.250, -119856.438);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * framenr / 300.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 300.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 7: {
		//FVector Pos2 = FVector(97568.750, 620657.250, -117801.898);
		//FVector Pos2 = FVector(95185.313, 641331.438, -116694.117);
		FVector Pos2 = FVector(96444.648, 634757.688, -117495.625);
		//FVector Pos2 = FVector(100944.430, 607356.250, -118364.445);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * framenr / 800.0f;
		FRotator NewRot = (Pos2 - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 800.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 8: {
		FVector Pos1 = FVector(174994.547, 675927.188, -86679.375);
		FVector Pos2 = FVector(-19848.641, 608025.688, -110734.070);
		FVector NewLoc = Pos1 + (Pos2 - Pos1) * framenr / 1200.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 1200.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 9: {
		//FVector Pos2 = FVector(18697.531, 643321.875, -118784.719);
		FVector Pos2 = FVector(64852.566, 650231.813, -120194.289);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * framenr / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 600.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
		}
		break;
	}
	case 10: {
		//FVector Pos2 = FVector(25300.885, 646003.500, -120586.031);
		FVector Pos2 = FVector(74327.547, 642531.375, -116940.477);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * framenr / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (framenr == 600.0) {
			framenr = 0.0;
			playerlastpos = NewLoc;
			scenenr += 1;
			//FVector NewLoc = FVector(97886.914, 644859.750,-115223.320);
			FVector NewLoc = FVector(97823.852, 653343.125, -111936.500);
			FRotator NewRot = (centerposdown - NewLoc).Rotation();
			PlayerPawn->TeleportTo(NewLoc, NewRot);
			this->Destroy();
		}
		break;
	}
	
	}
	
	
	
	
	
	
	//FVector NewLoc = centerpos + rotate.RotateVector(centerpos - playerstartpos);
	//FVector NewLoc = playerpos + (centerpos - playerpos) * framenr / 1000.0f;
	
	//FRotator NewRot = FRotator(0, 0, 0);
	//PlayerPawn->TeleportTo(NewLoc, NewRot);
	//PlayerPawn->SetActorLocation(NewLoc);
	//PlayerPawn->SetActorRotation(0,0,l0)

	//ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(world, 0);

	//myCharacter->SetActorLocation(playerpos + rotate.RotateVector(centerpos - playerpos));

}

