// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "PianoWorldSettings.generated.h"
/**
 * 
 */
UCLASS()
class UNREALPIANOFALL_API APianoWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
	FVector location = FVector(0.0f, 0.0f, 0.0f);
	FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
	
	
};
