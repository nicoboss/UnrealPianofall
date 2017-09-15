// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraManager.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include "Engine.h"



// Sets default values
ACameraManager::ACameraManager()
{
	//this->Destroy();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	//this->Destroy();

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
	else {
		camerapos_first_scene *= camera_speed;
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

	float tp_block_x = 14000.0f;
	float tp_block_y = 0.0;
	float tp_block_z = 10000.0f;
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
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//PlayerPawn->DisableInput(PlayerController);

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

	switch (scenenr) {
	case 0: {
		//Teleportion due to affect of opposite gravity
		
		FVector NewLoc = FVector(40000.0, 0.0, 32000.0);
		FRotator NewRot = (centerposmid - NewLoc).Rotation();
		PlayerPawn->SetActorLocation(NewLoc);
		PlayerController->SetControlRotation(NewRot);
		if (camerapos >= camerapos_first_scene) {
			//For repeate in case it jumps to scene 0 (which it doesn't by default)
			camerapos_first_scene -= frames_wait_for_camera_fix * camera_speed;
			camerapos = 0.0;
			playerstartpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 1: {
		FRotator rotate = FRotator((camerapos / 10.0)-170, 0, 0);
		FVector NewLoc = centerpos + rotate.RotateVector(centerpos - playerstartpos);
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->SetActorLocation(NewLoc);
		PlayerController->SetControlRotation(NewRot);
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
		PlayerPawn->SetActorLocation(NewLoc);
		PlayerController->SetControlRotation(NewRot);
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
		FRotator NewRot = (centerposmid - NewLoc).Rotation();
		PlayerPawn->SetActorLocation(NewLoc);
		PlayerController->SetControlRotation(NewRot);
		if (camerapos >= 1000.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			++scenenr;
		}
		break;
	}
	case 4: {
		//FVector Pos2 = FVector(25300.885, 646003.500, -120586.031);
		FVector Pos2 = FVector(0, 0, 2000);
		FVector NewLoc = playerlastpos + (Pos2 - playerlastpos) * camerapos / 600.0f;
		FRotator NewRot = (centerposdown - NewLoc).Rotation();
		PlayerPawn->SetActorLocation(NewLoc);
		PlayerController->SetControlRotation(NewRot);
		if (camerapos >= 600.0) {
			camerapos = 0.0;
			playerlastpos = NewLoc;
			//FVector NewLoc = FVector(97886.914, 644859.750,-115223.320);
			FVector NewLoc = FVector(0, 0, 2000);
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

