// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockGenerator.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include "Engine.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Public/ImageUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <fstream>
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

	std::array<uint32, 128> testArray = {};
	testArray[77] = 777;
	spawnpos.push_back(testArray); //Copy testArray into spawnpos

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

	//std::ifstream midifile("A:\\Music\\britney_spears-born_to_make_you_happy.mid", std::ios::binary); //Huge amount of errors
	//std::ifstream midifile("A:\\Music\\ZELDA.MID", std::ios::binary); //No errors
	std::ifstream midifile("A:\\Music\\la_isla_bonita.mid", std::ios::binary); //Huge amount of errors
	//std::ifstream midifile("A:\\Music\\05ClassExample60bpm.mid", std::ios::binary); //No errors

	uint16 PPQ;
	uint32 usPQ = 500000; //120 PBM

	uint8 Length_len = 0;
	uint32 MTrk_len = 0;

	bool in_deltatime;
	uint64 time_us = 0;
	uint32 deltatime_to_add;
	uint8 deltatime_data_i;
	uint8 deltatime_data_size = 0;
	uint8 deltatime_data[5] = {};

	int8 Note_to_play = -1;
	int8 Note_to_play_nr = -1;
	int8 Note_to_stop = -1;
	int8 Note_to_stop_nr = -1;

	int8 Pressure_to_change = -1;
	int8 Pressure_note = -1;
	uint8 Pressure_value;
	int8 Controller_to_change = -1;
	int8 Controller_type = -1;
	uint8 Controller_value;
	int8 Program_to_change = -1;
	int8 Instument_table[16] = { 0 };
	int8 Channel_pressure_to_change = -1;
	int8 Pressure_table[16] = { 0 };
	int8 Pitch_to_change = -1;
	int8 Pitch_lsb = -1;
	uint16 Pitch_value;
	bool in_SysEx = false;
	uint8 SysEx_type;
	int16 SysEx_len;
	bool SysEx_escape_seq = true;
	bool in_META = false;
	int16 META_type = -1;
	int16 META_len = -1;
	uint8 MIDI_data_size = 0;
	uint8 MIDI_data_pos = 0;
	uint8 META_data[255] = {};
	uint16 MIDI_data_i;

	char MTrk[4] = { 'M','T','r','k' };
	uint8 MTrk_findpos = 0;
	uint16 pos = 0;
	std::stringstream ss;
	int32 MTrk_nr = -1;
	char filebuf[4096] = { 0 };

	char hexstr[100];

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
		PPQ = (header[12] << 8) + header[13];
		if (PPQ == 0) {
			PPQ = 120;
		}
		UE_LOG(LogTemp, Log, TEXT("PPQ: %u"), PPQ);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MIDI-File: Invalid header"));
		UE_LOG(LogTemp, Error, TEXT("Invalid MIDI-file!"));
	}





	do {
		UE_LOG(LogTemp, Log, TEXT("Read Block"));
		midifile.read(filebuf, 4096);
		unsigned char *buf = (unsigned char *)filebuf;
		pos = 0;

		read_next_MTrk:

		if (MTrk_len == 0) {
			for (; pos < 4096; ++pos) {
				if (buf[pos] == MTrk[MTrk_findpos]) {
					if (++MTrk_findpos == 4) {
						UE_LOG(LogTemp, Log, TEXT("MTrk Found!"));
						usPQ = 500000; //120 PBM
						time_us = 0;
						MTrk_findpos = 0;
						++MTrk_nr;
						MTrk_len = 0;
						Length_len = 4;
						deltatime_data_size = 0;
						Note_to_play = -1;
						Note_to_play_nr = -1;
						Note_to_stop = -1;
						Note_to_stop_nr = -1;
						Pressure_to_change = -1;
						Controller_to_change = -1;
						Controller_type = -1;
						Program_to_change = -1;
						Channel_pressure_to_change = -1;
						Pitch_to_change = -1;
						Pitch_lsb = -1;
						in_SysEx = false;
						SysEx_len = -1;
						SysEx_escape_seq = true;
						in_META = false;
						META_type = -1;
						META_len = -1;
						++pos;
						break;
					}
				}
 else {
	 if (buf[pos] == MTrk[0]) {
		 MTrk_findpos = 1;
	 }
	 else {
		 MTrk_findpos = 0;
	 }
 }
			}
		}

		UE_LOG(LogTemp, Log, TEXT("pos: %u"), pos);

		if (Length_len > 0) {
			for (; pos < 4096; ++pos) {
				_itoa((uint8)buf[pos], hexstr, 16);
				UE_LOG(LogTemp, Log, TEXT("Length Data: %s"), ANSI_TO_TCHAR(hexstr));
				MTrk_len += buf[pos] << (--Length_len * 8);
				if (Length_len == 0) {
					UE_LOG(LogTemp, Log, TEXT("MTrk_len: %u"), MTrk_len);
					++pos;
					in_deltatime = true;
					break;
				}
			}

		}

		if (MTrk_len > 0) {
			for (; pos < 4096; ++pos) {
				_itoa((uint8)buf[pos], hexstr, 16);
				UE_LOG(LogTemp, Log, TEXT("MTrk_len: %u Data: %s = %d"), MTrk_len, ANSI_TO_TCHAR(hexstr), buf[pos]);
				if (--MTrk_len == 0) {
					UE_LOG(LogTemp, Log, TEXT("META-End!"));
					if (in_META == false || META_type != 0x2F) {
						UE_LOG(LogTemp, Warning, TEXT("Invalid end-of-track MIDI-event!"));
					}
					in_META = false;
					goto read_next_MTrk;
				}

				if (in_deltatime == true) {
					deltatime_data[deltatime_data_size++] = buf[pos] & 0b01111111;
					if (buf[pos] < 0b10000000) {
						deltatime_data[deltatime_data_size] = buf[pos];
						deltatime_to_add = 0;
						for (deltatime_data_i = 0; deltatime_data_i < deltatime_data_size; ++deltatime_data_i) {
							deltatime_to_add += deltatime_data[deltatime_data_i] << ((deltatime_data_size - deltatime_data_i - 1) * 8);
						}
						time_us += (deltatime_to_add*usPQ)/PPQ;
						deltatime_data_size = 0;
						UE_LOG(LogTemp, Log, TEXT("deltatime_to_add: %u"), deltatime_to_add);
						UE_LOG(LogTemp, Log, TEXT("Time: %f"), time_us/1000000.0);
						in_deltatime = false;
					}
					continue;
				}


				if (in_META == true) {
					if (META_type == -1) {
						UE_LOG(LogTemp, Log, TEXT("META_type = %u"), buf[pos]);
						META_type = buf[pos];
					}
					else {
						if (META_len == -1) {
							UE_LOG(LogTemp, Log, TEXT("META_len = %u"), buf[pos]);
							META_len = buf[pos];
							MIDI_data_size = buf[pos];
						}
						else {
							--META_len;
							META_data[MIDI_data_pos++] = buf[pos];
							if (META_len == 0) {
								--MIDI_data_pos;
								if (META_type == 0x51) {
									usPQ = 0;
									for (MIDI_data_i = 0; MIDI_data_i < MIDI_data_size; ++MIDI_data_i) {
										usPQ += META_data[MIDI_data_i] << ((MIDI_data_size - MIDI_data_i - 1) * 8);
									}
									if (usPQ == 0) {
										usPQ = 500000; //120 PBM
									}
									UE_LOG(LogTemp, Log, TEXT("New usPQ = %u"), usPQ);
								}
								UE_LOG(LogTemp, Log, TEXT("META-End!"));
								in_META = false;
								in_deltatime = true;
							}
						}
					}
				}

				//Take a look at http://www.somascape.org/midi/tech/mfile.html to understand it
				else if (buf[pos] >= 0b10000000) {
					if (buf[pos] >= 0x80 && buf[pos] <= 0x8F) {
						UE_LOG(LogTemp, Log, TEXT("NoteOFF: Channel %u"), buf[pos] - 0x80);
						Note_to_stop = buf[pos] - 0x80;
					}
					else if (buf[pos] >= 0x90 && buf[pos] <= 0x9F) {
						UE_LOG(LogTemp, Log, TEXT("NoteON: Channel %u"), buf[pos] - 0x90);
						Note_to_play = buf[pos] - 0x90;
					}
					else if (buf[pos] >= 0xA0 && buf[pos] <= 0xAF) {
						UE_LOG(LogTemp, Log, TEXT("Polyphonic Pressure: Channel %u"), buf[pos] - 0x90);
						Pressure_to_change = buf[pos] - 0xA0;
					}
					else if (buf[pos] >= 0xB0 && buf[pos] <= 0xBF) {
						UE_LOG(LogTemp, Log, TEXT("Controller Change: Channel %u"), buf[pos] - 0xB0);
						Controller_to_change = buf[pos] - 0xB0;
					}
					else if (buf[pos] >= 0xC0 && buf[pos] <= 0xCF) {
						UE_LOG(LogTemp, Log, TEXT("Program Change: Channel %u"), buf[pos] - 0xC0);
						Program_to_change = buf[pos] - 0xC0;
					}
					else if (buf[pos] >= 0xD0 && buf[pos] <= 0xDF) {
						UE_LOG(LogTemp, Log, TEXT("Channel Pressure Change: Channel %u"), buf[pos] - 0xC0);
						Channel_pressure_to_change = buf[pos] - 0xD0;
					}
					else if (buf[pos] >= 0xE0 && buf[pos] <= 0xEF) {
						UE_LOG(LogTemp, Log, TEXT("Pitch Bend: Channel %u"), buf[pos] - 0xC0);
						Pitch_to_change = buf[pos] - 0xE0;
					}
					else if (buf[pos] == 0xF0 || buf[pos] == 0xF7) {
						UE_LOG(LogTemp, Log, TEXT("SysEx/EscSeq:"), buf[pos] - 0xC0);
						SysEx_type = buf[pos];
						in_SysEx = true;
					}
					else if (buf[pos] == 0xFF) {
						UE_LOG(LogTemp, Log, TEXT("META-Start"));
						MIDI_data_pos = 0;
						META_len = -1;
						META_type = -1;
						in_META = true;
					}
					else {
						UE_LOG(LogTemp, Log, TEXT("Error: Unknown MIDI Event %u"), buf[pos]);
					}
				}

				else {
					if (Note_to_play != -1) {
						if (Note_to_play_nr == -1) {
							Note_to_play_nr = buf[pos];
							_itoa((uint8)buf[pos], hexstr, 16);
							UE_LOG(LogTemp, Log, TEXT("NoteON: Nr: %s"), ANSI_TO_TCHAR(hexstr));
						}
						else {
							Note_to_play = -1;
							Note_to_play_nr = -1;
							in_deltatime = true;
						}
					}
					else if (Note_to_stop != -1) {
						if (Note_to_stop_nr == -1) {
							Note_to_stop_nr = buf[pos];
							_itoa((uint8)buf[pos], hexstr, 16);
							UE_LOG(LogTemp, Log, TEXT("NoteOFF: Nr: %s"), ANSI_TO_TCHAR(hexstr));
						}
						else {
							Note_to_stop = -1;
							Note_to_stop_nr = -1;
							in_deltatime = true;
						}
					}
					else if (Pressure_to_change != -1) {
						if (Pressure_note == -1) {
							Pressure_note = buf[pos];
						}
						else {
							Pressure_value = buf[pos];
							Pressure_note = -1;
							Pressure_to_change = -1;
							in_deltatime = true;
						}
					}
					else if (Controller_to_change != -1) {
						if (Controller_type == -1) {
							Controller_type = buf[pos];
						}
						else {
							Controller_value = buf[pos];
							Controller_type = -1;
							Controller_to_change = -1;
							in_deltatime = true;
						}
					}
					else if (Program_to_change != -1) {
						UE_LOG(LogTemp, Log, TEXT("Programm Change: Channel %d = %u"), Program_to_change, buf[pos]);
						Instument_table[Program_to_change] = buf[pos];
						Program_to_change = -1;
						in_deltatime = true;
					}
					else if (Channel_pressure_to_change != -1) {
						Pressure_table[Channel_pressure_to_change] = buf[pos];
						Channel_pressure_to_change = -1;
						in_deltatime = true;
					}
					else if (Pitch_to_change != -1) {
						if (Pitch_lsb == -1) {
							Pitch_lsb = buf[pos];
						}
						else {
							Pitch_value = (buf[pos] << 7) + Pitch_lsb;
							Pitch_lsb = -1;
							Pitch_to_change = -1;
							in_deltatime = true;
						}
					}
					else if (in_SysEx == true) {
						if (SysEx_len == -1) {
							UE_LOG(LogTemp, Log, TEXT("SysEx_len = %u"), buf[pos]);
							SysEx_len = buf[pos];
							MIDI_data_size = buf[pos];
						}
						else {
							--SysEx_len;
							if (SysEx_len == 0) {
								if (buf[pos] == 0xF7) {
									SysEx_escape_seq = true;
								} else {
									SysEx_escape_seq = false;
								}
								SysEx_len = -1;
								in_SysEx = false;
							}
						}
					}
					else {
						UE_LOG(LogTemp, Log, TEXT("Error: Unknown MIDI Data %u"), buf[pos]);
					}
				}
			}
		}


	} while (midifile);

}

// Called every frame
void ABlockGenerator::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Log, TEXT("spawnpos[0][77]: %u"), spawnpos[0][77]);
	Super::Tick(DeltaTime);
	++FrameNr;

	int32 notenr_rnd;

	for (int i = 0; i < 100; ++i) {
		notenr_rnd = (uint8)FMath::RandRange(0.0f, 127.0f);
		FVector location = FVector((float)(6400 - notenr_rnd * 100), 0.0f, 4000.0f);
		FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = this;
		SpawnInfo.Name = *FString::Printf(TEXT("F%uN%u"), FrameNr, notenr_rnd);
		AActor* newBlock = GetWorld()->SpawnActor<ABlock>(ABlock::StaticClass(), location, rotate, SpawnInfo);
	}


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

