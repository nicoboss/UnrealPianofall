// Fill out your copyright notice in the Description page of Project Settings.

#include "GDC_Demo.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include "Engine.h"
#include "CameraManager.h"

//#define HOUSE_EXPLOSION


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

	camera_fix = (FParse::Param(FCommandLine::Get(), TEXT("-fix")))
					| (FParse::Param(FCommandLine::Get(), TEXT("fix")));

	float arg_camera_speed;
	if (FParse::Value(FCommandLine::Get(), TEXT("speed"), arg_camera_speed)) {
		camera_speed = arg_camera_speed;
	}

	float arg_seconds_wait_for_camera_load;
	if (FParse::Value(FCommandLine::Get(), TEXT("wait"), arg_seconds_wait_for_camera_load)) {
		frames_wait_for_camera_fix = (int)(arg_seconds_wait_for_camera_load * 60.0f);
		camerapos_first_scene = arg_seconds_wait_for_camera_load * 60.0f * camera_speed + 120.0f;
	}
	
	camera_no_repeat = (FParse::Param(FCommandLine::Get(), TEXT("-no-repeat")))
					| (FParse::Param(FCommandLine::Get(), TEXT("no-repeat")));


	if (FParse::Param(FCommandLine::Get(), TEXT("-free"))
		|| FParse::Param(FCommandLine::Get(), TEXT("free"))) {
		this->Destroy();
		return;
	}

}


// Called when the game ends or when despawned
void ACameraManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	float tp_block_x = 90600.0f;
	float tp_block_y = 645000.0;
	float tp_block_z = -110000.0f;
	FParse::Value(FCommandLine::Get(), TEXT("block_x"), tp_block_x);
	FParse::Value(FCommandLine::Get(), TEXT("block_y"), tp_block_y);
	FParse::Value(FCommandLine::Get(), TEXT("block_z"), tp_block_z);
	FVector tp_vector(tp_block_x, tp_block_y, tp_block_z);

	if (FParse::Param(FCommandLine::Get(), TEXT("-tp"))
		|| FParse::Param(FCommandLine::Get(), TEXT("tp"))) {
		APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		PlayerPawn->SetActorLocation(tp_vector);
	}

}


// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	UWorld* world = GetWorld();
	APlayerController* currentPlayerController = world->GetFirstPlayerController();

	//Exit the Camera if the player had pressed End in the last frame
	if (currentPlayerController->WasInputKeyJustPressed(EKeys::End)) {
		this->Destroy();
	}

	APawn* PlayerPawn = currentPlayerController->GetPawn();
	if (camera_fix == true) {
		if (camera_fix_framecount <= frames_wait_for_camera_fix) {
			if (camera_fix_framecount == frames_wait_for_camera_fix) {
				camera_fix_vector = PlayerPawn->GetActorLocation();
				camera_fix_rotator = PlayerPawn->GetActorRotation();
			}
			++camera_fix_framecount;
		} else {
			PlayerPawn->TeleportTo(camera_fix_vector, camera_fix_rotator);
		}
		return;
	}

	camerapos += 1.0 * camera_speed;

	//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, "Scene: " + FString::FromInt(scenenr) + " camerapos: " + FString::FromInt(camerapos));
	//this->Destroy();

	#ifdef HOUSE_EXPLOSION
		//Wrecking Ball House Explosion Scene
		//Startpos: -69217.508   207054.313   -122535.336
		switch (scenenr) {
		case 0: {
			//Teleportion due to affect of opposite gravity
			FRotator NewRot = (FVector(-74520.0, 277950.0, -121770.0) - FVector(-69217.508, 297954.313, -122534.336)).Rotation();
			PlayerPawn->TeleportTo(FVector(-69217.508, 297954.313, -122534.336), NewRot);
			if (camerapos >= camerapos_first_scene - 120.0f) {
				camerapos = 0.0;
				++scenenr;
			}
			break;
		}
		case 1: {
			FVector Pos1 = FVector(-69217.508, 297954.313, -122534.336);
			FVector Pos2 = FVector(-74572.813, 280300.0, -120000.0);
			FVector NewLoc = Pos1 + (Pos2 - Pos1) * camerapos / 900.0;
			FRotator NewRot = (FVector(-74520.0, 277950.0, -121770.0) - NewLoc).Rotation();
			PlayerPawn->TeleportTo(NewLoc, NewRot);
			if (camerapos >= 900.0) {
				camerapos = 0.0;
				playerlastpos = NewLoc;
				++scenenr;
			}
			break;
		}
		case 2: {
			FVector Pos2 = FVector(-74983.930, 280599.625, -71643.664);
			FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * camerapos / 800.0f;
			FRotator NewRot = (playerlastpos - NewLoc).Rotation();
			PlayerPawn->TeleportTo(NewLoc, NewRot);
			if (camerapos >= 1500.0) {
				camerapos = 0.0;
				playerlastpos = NewLoc;
				++scenenr;
				this->Destroy();
			}
			break;
		}
		}
		return;
	#endif

	switch (scenenr) {
	case 0: {
		//Teleportion due to affect of opposite gravity
		PlayerPawn->TeleportTo(FVector(102225.0, 647403.0, -113628.0), FRotator(1.999872, 113628.0, 0.000002));
		if (camerapos >= camerapos_first_scene) {
			//For repeate in case it jumps to scene 0 (which it doesn't by default)
			camerapos_first_scene -= frames_wait_for_camera_fix * camera_speed;
			camerapos = 0.0;
			++scenenr;
		}
		break;
	}
	case 1: {
		FRotator rotate = FRotator((camerapos / 10.0)-170, 0, 0);
		FVector NewLoc = centerpos + rotate.RotateVector(centerpos - playerstartpos);
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 1800.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 2: {
		FVector NewLoc = playerlastpos + (playerstartpos - playerlastpos) * camerapos / 600.0f;
		FRotator NewRot = (centerpos - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 650.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 3: {
		FRotator rotate = FRotator(0, camerapos / 4.0, 0);
		FVector NewLoc = centerpos + rotate.RotateVector(centerpos - playerstartpos);
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 1000.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 4: {
		FVector NewLoc = playerlastpos + (playerstartpos - playerlastpos) * camerapos / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 600.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 5: {
		FVector Pos1 = FVector(38622.285, 703740.188, -107188.945);
		FVector Pos2 = FVector(40000.000, 703400.875, -107189.617);
		FVector NewLoc = Pos1 + (Pos2 - Pos1) * camerapos / 60.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 60.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 6: {
		FVector Pos2 = FVector(59940.891, 680811.250, -119856.438);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * camerapos / 300.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 300.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 7: {
		//FVector Pos2 = FVector(97568.750, 620657.250, -117801.898);
		//FVector Pos2 = FVector(95185.313, 641331.438, -116694.117);
		FVector Pos2 = FVector(96444.648, 634757.688, -117495.625);
		//FVector Pos2 = FVector(100944.430, 607356.250, -118364.445);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * camerapos / 800.0f;
		FRotator NewRot = (Pos2 - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 800.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 8: {
		FVector Pos1 = FVector(174994.547, 675927.188, -86679.375);
		FVector Pos2 = FVector(-19848.641, 608025.688, -110734.070);
		FVector NewLoc = Pos1 + (Pos2 - Pos1) * camerapos / 1200.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 1200.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 9: {
		//FVector Pos2 = FVector(18697.531, 643321.875, -118784.719);
		FVector Pos2 = FVector(64852.566, 650231.813, -120194.289);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * camerapos / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 600.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 10: {
		//FVector Pos2 = FVector(25300.885, 646003.500, -120586.031);
		FVector Pos2 = FVector(74327.547, 642531.375, -116940.477);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * camerapos / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->TeleportTo(NewLoc, NewRot);
		if (camerapos >= 600.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			//FVector NewLoc = FVector(97886.914, 644859.750,-115223.320);
			FVector NewLoc = FVector(97823.852, 653343.125, -111936.500);
			FRotator NewRot = (centerposdown - NewLoc).Rotation();
			PlayerPawn->TeleportTo(NewLoc, NewRot);
			if (camera_no_repeat == false) {
				scenenr = 1;
			} else {
				++scenenr;
				this->Destroy();
			}
		}
		break;
	}
	
	}
}

