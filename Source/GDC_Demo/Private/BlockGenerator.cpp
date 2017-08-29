// Fill out your copyright notice in the Description page of Project Settings.

#include "GDC_Demo.h"
#include "BlockGenerator.h"
#include "Engine/World.h"
#include <EngineGlobals.h>
#include "Engine.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Public/ImageUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <fstream>
#include "Block.h"


#define MIDI_OUT 0
#if WITH_EDITOR == 1
#define MIDI_PATH "A:\\Music\\BA_Rare_ASDF_Mode_rev_1.1.mid"
#define LIMIT_VALUE 6000
#define REDUCTION_VALUE 5
#endif
#define LOG_PATH "A:\\Music\\midi_data.csv"

#ifdef LOG_PATH
#define LOG_NOTES 0
#define LOG_EVENTS 1
#else
#define LOG_NOTES 0 
#define LOG_EVENTS 0
#endif

#if MIDI_OUT == 1 && WITH_EDITOR == 0
#include "../MidiInterface/Classes/RtMidi.h"
#include <thread>
#endif

#include <iostream>
#include <cstdlib>

#if MIDI_OUT == 1 && WITH_EDITOR == 0
	RtMidiOut *midiout = 0;
#endif
std::vector<unsigned char> message;

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

	//File compatible with http://www.fourmilab.ch/webtools/midicsv/
	//It's very usefull to test my MIDI pharser
	//Binary mode in order to get linux line-endings due to sed output has linux line endings
	//How to conveart the midicsv output to MIDI note events only so that thair hashes should match:
	//sed '/Note_on_c\|Note_off_c/!d' 05ClassExample60bpm.csv > 05ClassExample60bpm_notes.csv
	#ifdef LOG_PATH
		#if LOG_NOTES == 1 || LOG_EVENTS == 1
			std::ofstream logfile(LOG_PATH, std::ofstream::trunc | std::ios::binary);
		#endif
	#endif

	const FText cmd_error_title = FText::FromName("Invalid command line argument");
	FString arg_midi_fileName;
	FString midi_fileName;
	uint16 arg_spawnreduction;
	uint32 arg_blocklimit;

	#ifdef MIDI_PATH
		midi_fileName = MIDI_PATH;
	#else
		if (FParse::Value(FCommandLine::Get(), TEXT("midi"), arg_midi_fileName)) {
			midi_fileName = arg_midi_fileName.Replace(TEXT("="), TEXT("")).Replace(TEXT("\""), TEXT("")); // replace quotes
		
		}
		else {
			//GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
			//FGenericPlatformMisc::RequestExit(true);
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Invalid MIDI-file!\nCommand line argument missing!\nExample how to call this software:\n-midi=C:\\Music\\BA_Rare_ASDF_Mode_rev_1.1.mid"), &cmd_error_title);
		}
	#endif
	
	
	#ifdef LIMIT_VALUE
		blocklimit = LIMIT_VALUE;
	#else
		if (FParse::Value(FCommandLine::Get(), TEXT("limit"), arg_blocklimit)) {
			blocklimit = arg_blocklimit;
		}
	#endif


	#ifdef REDUCTION_VALUE
		spawnreduction = REDUCTION_VALUE;
	#else
		if (FParse::Value(FCommandLine::Get(), TEXT("reduction"), arg_spawnreduction)) {
			spawnreduction = arg_spawnreduction;
		}
	#endif



	std::ifstream midifile(*midi_fileName, std::ios::binary);

	std::string portName;
#if MIDI_OUT == 1 && WITH_EDITOR == 0
	midiout = new RtMidiOut();


	UE_LOG(LogTemp, Log, TEXT("midiout->getPortCount(): %u"), midiout->getPortCount());
	portName = midiout->getPortName(0);
	UE_LOG(LogTemp, Log, TEXT("midiout->getPortName(0): %s"), *FString(portName.c_str()));
	midiout->openPort(0);

	// Program change: 192, 5
	message.push_back(0xC0);
	message.push_back(0x00);
	midiout->sendMessage(&message);
	message.push_back(0);

	//message[0] = 0xF1;
	//message[1] = 60;
	//midiout->sendMessage(&message);

	// Control Change: 176, 7, 100 (volume)
	//message[0] = 176;
	//message[1] = 7;
	//message.push_back(100);
	//midiout->sendMessage(&message);

	// Note On: 144, 64, 90
	message[0] = 0x90;
	message[1] = 64;
	message[2] = 90;
	midiout->sendMessage(&message);

	std::this_thread::sleep_for(std::chrono::milliseconds(420)); //c++11

																 // Note Off: 128, 64, 40
	message[0] = 0x80;
	message[1] = 64;
	message[2] = 0;
	midiout->sendMessage(&message);

	std::this_thread::sleep_for(std::chrono::milliseconds(420)); //c++11
#endif

	uint16 PPQ; //Remember: Never reset this between MTrks
	uint32 usPQ = 500000; //120 PBM because 60000000/500000=500000

	uint8 Length_len = 0;
	uint32 MTrk_len = 0;

	uint8 running_status = 0;

	bool in_deltatime;
	uint64 tick = 0;
	uint64 time_us = 0;
	uint32 frame_nr = 0;
	std::array<uint32, 128> zeroArray = { 0 };
	//zeroArray.fill(1);
	//zeroArray.fill(10);
	//zeroArray[0] = 1;
	//zeroArray[31] = 5;
	//zeroArray[63] = 10;
	//zeroArray[95] = 15;
	//zeroArray[127] = 20;

	spawnpos.push_back(zeroArray);
	stoppos.push_back(zeroArray);

	uint32 deltatime_to_add;
	uint8 deltatime_data_i;
	uint8 deltatime_data_size = 0;
	uint8 deltatime_data[5] = {};

	int8 Note_to_play_nr = -1;
	int8 Note_to_stop_nr = -1;

	//int8 Pressure_to_change = -1;
	int8 Pressure_note = -1;
	uint8 Pressure_value;
	//int8 Controller_to_change = -1;
	int8 Controller_type = -1;
	uint8 Controller_value;
	//int8 Program_to_change = -1;
	int8 Instument_table[16] = { 0 };
	//int8 Channel_pressure_to_change = -1;
	int8 Pressure_table[16] = { 0 };
	//int8 Pitch_to_change = -1;
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
	//If end of file in for loop: count to end of block
	uint32 pos = 0; //1 MB buffer = 1048576
	std::stringstream ss;
	uint32 MTrk_nr = 0; //First MTrk is number 1 to fit the midicsv standard
	char filebuf[1048576] = { 0 };

	char hexstr[100];

	char headerbuf[14];
	midifile.read(headerbuf, 14);
	unsigned char *header = (unsigned char *)headerbuf;
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
			UE_LOG(LogTemp, Error, TEXT("Error: PPQ=0 => set to default value: 120"));
			PPQ = 120;
		}
		UE_LOG(LogTemp, Log, TEXT("PPQ: %u"), PPQ);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MIDI-File: Invalid header"));
		UE_LOG(LogTemp, Error, TEXT("Invalid MIDI-file!"));
	}





	do {
		//UE_LOG(LogTemp, Log, TEXT("Read Block"));
		UE_LOG(LogTemp, Log, TEXT("Status: spawnpos list size: %u"), spawnpos.size());
		UE_LOG(LogTemp, Log, TEXT("Status: MTrk time [s]: %u"), time_us / 1000000);
		midifile.read(filebuf, 1048576);
		unsigned char *buf = (unsigned char *)filebuf;
		pos = 0;
		//UE_LOG(LogTemp, Log, TEXT("Pos set to 0"), pos);

	read_next_MTrk:
		//UE_LOG(LogTemp, Log, TEXT("read_next_MTrk Pos: %u"), pos);


		if (MTrk_len == 0) {
			for (; pos < 1048576; ++pos) {
				//UE_LOG(LogTemp, Log, TEXT("search MTrk Pos: %u"), pos);
				if (buf[pos] == MTrk[MTrk_findpos]) {
					if (++MTrk_findpos == 4) {
						++MTrk_nr;
						#if LOG_EVENTS == 1
							logfile << "MTrk Found! MTrk_nr: " << MTrk_nr << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("MTrk Found! MTrk_nr: %u"), MTrk_nr);
						tick = 0;
						time_us = 0;
						//usPQ = 500000; //NO NEVER DO THIS! IT'S WRONG! (No idea why)
						frame_nr = 0;
						MTrk_findpos = 0;
						MTrk_len = 0;
						Length_len = 4;
						running_status = 0;
						deltatime_data_size = 0;
						Note_to_play_nr = -1;
						Note_to_stop_nr = -1;
						Controller_type = -1;
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

		//UE_LOG(LogTemp, Log, TEXT("pos: %u"), pos);

		if (Length_len > 0) {
			for (; pos < 1048576; ++pos) {
				_itoa((uint8)buf[pos], hexstr, 16);
				#if LOG_EVENTS == 1
					logfile << "Length Data: " << hexstr << "\n";
				#endif
				UE_LOG(LogTemp, Log, TEXT("Length Data: %s"), ANSI_TO_TCHAR(hexstr));
				MTrk_len += buf[pos] << (--Length_len * 8);
				if (Length_len == 0) {
					#if LOG_EVENTS == 1
						logfile << "MTrk_len: " << (int)MTrk_len << "\n";
					#endif
					UE_LOG(LogTemp, Log, TEXT("MTrk_len: %u"), MTrk_len);
					++pos;
					in_deltatime = true;
					break;
				}
			}

		}

		if (MTrk_len > 0) {
			for (; pos < 1048576; ++pos) {
				if (MTrk_nr >= 37 && MTrk_nr <= 38 || MTrk_nr <= 2) {
					_itoa((uint8)buf[pos], hexstr, 16);
					#if LOG_EVENTS == 1
						logfile << "MTrk_len: " << (int)MTrk_len << " Data: " << hexstr << " = " << (int)buf[pos] << " Pos: " << (int)pos << "\n";
					#endif
					//UE_LOG(LogTemp, Log, TEXT("MTrk_len: %u Data: %s = %d Pos: %u"), MTrk_len, ANSI_TO_TCHAR(hexstr), buf[pos], pos);
				}

				if (--MTrk_len == 0) {
					#if LOG_EVENTS == 1
						logfile << "MTrk-End!\n";
						logfile << "Frames: " << frame_nr << "\n";
					#endif
					UE_LOG(LogTemp, Log, TEXT("MTrk-End!"));
					UE_LOG(LogTemp, Log, TEXT("Frames: %u"), frame_nr);
					if (in_META == false || META_type != 0x2F) {
						#if LOG_EVENTS == 1
							logfile << "Invalid end-of-track MIDI-event!\n";
						#endif
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
							deltatime_to_add += deltatime_data[deltatime_data_i] << ((deltatime_data_size - deltatime_data_i - 1) * 7);
						}
						tick += deltatime_to_add;
						time_us += (deltatime_to_add*usPQ) / PPQ;
						frame_nr = floor(time_us*0.00006);
						while (spawnpos.size() <= frame_nr) {
							spawnpos.push_back(zeroArray); //Copy testArray into spawnpos
							stoppos.push_back(zeroArray); //Copy testArray into spawnpos
						}
						deltatime_data_size = 0;
						//UE_LOG(LogTemp, Log, TEXT("deltatime_to_add: %u"), deltatime_to_add);
						//UE_LOG(LogTemp, Log, TEXT("Time: %f"), time_us/1000000.0);
						in_deltatime = false;
					}
					continue;
				}


				if (in_META == true) {
					if (META_type == -1) {
						#if LOG_EVENTS == 1
							logfile << "META_type = " << (int)buf[pos] << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("META_type = %u"), buf[pos]);
						META_type = buf[pos];
					}
					else {
						if (META_len == -1) {
							#if LOG_EVENTS == 1
								logfile << "META_len = " << (int)buf[pos] << "\n";
							#endif
							UE_LOG(LogTemp, Log, TEXT("META_len = %u"), buf[pos]);
							META_len = buf[pos];
							MIDI_data_size = META_len;
							if (META_len == 0) {
								goto META_end_label;
							}
						}
						else {
							--META_len;
							META_data[MIDI_data_pos++] = buf[pos];
							if (META_len == 0) {
								--MIDI_data_pos;
								META_end_label:
								if (META_type == 0x51) {
									usPQ = 0;
									for (MIDI_data_i = 0; MIDI_data_i < MIDI_data_size; ++MIDI_data_i) {
										usPQ += META_data[MIDI_data_i] << ((MIDI_data_size - MIDI_data_i - 1) * 8);
										//UE_LOG(LogTemp, Log, TEXT("New usPQ data: %u"), META_data[MIDI_data_i]);
									}
									if (usPQ == 0) {
										usPQ = 500000; //120 PBM because 60000000/500000=500000
									}
									#if LOG_EVENTS == 1
										logfile << "New usPQ = " << (int)usPQ << "\n";
									#endif
									UE_LOG(LogTemp, Log, TEXT("New usPQ = %u"), usPQ);
								}
								#if LOG_EVENTS == 1
									logfile << "META-End!" << "\n";
								#endif
								UE_LOG(LogTemp, Log, TEXT("META-End!"));
								in_META = false;
								in_deltatime = true;
							}
						}
					}
				}

				//Take a look at http://www.somascape.org/midi/tech/mfile.html to understand it
				else if (buf[pos] >= 0b10000000) {
					if (buf[pos] <= 0x8F) {
						//UE_LOG(LogTemp, Log, TEXT("NoteOFF: Channel %u"), buf[pos] - 0x80);
						running_status = buf[pos];
					}
					else if (buf[pos] <= 0x9F) {
						//UE_LOG(LogTemp, Log, TEXT("NoteON: Channel %u"), buf[pos] - 0x90);
						running_status = buf[pos];
					}
					else if (buf[pos] <= 0xAF) {
						#if LOG_EVENTS == 1
							logfile << "Polyphonic Pressure: Channel " << (int)(buf[pos] - 0x90) << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("Polyphonic Pressure: Channel %u"), buf[pos] - 0x90);
						running_status = buf[pos];
					}
					else if (buf[pos] <= 0xBF) {
						#if LOG_EVENTS == 1
							logfile << "Controller Change: Channel " << (int)buf[pos] - 0xB0 << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("Controller Change: Channel %u"), buf[pos] - 0xB0);
						running_status = buf[pos];
					}
					else if (buf[pos] <= 0xCF) {
						running_status = buf[pos];
					}
					else if (buf[pos] <= 0xDF) {
						#if LOG_EVENTS == 1
							logfile << "Channel Pressure Change: Channel " << (int)buf[pos] - 0xC0 << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("Channel Pressure Change: Channel %u"), buf[pos] - 0xC0);
						running_status = buf[pos];
					}
					else if (buf[pos] <= 0xEF) {
						#if LOG_EVENTS == 1
						logfile << "Pitch Bend: Channel " << (int)buf[pos] - 0xC0 << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("Pitch Bend: Channel %u"), buf[pos] - 0xC0);
						running_status = buf[pos];
					}
					else if (buf[pos] == 0xF0 || buf[pos] == 0xF7) {
						#if LOG_EVENTS == 1
							logfile << "SysEx/EscSeq: " << (int)buf[pos] - 0xC0 << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("SysEx/EscSeq: "), buf[pos] - 0xC0);
						SysEx_type = buf[pos];
						in_SysEx = true;
						running_status = 0;
					}
					else if (buf[pos] == 0xFF) {
						#if LOG_EVENTS == 1
							logfile << "META-Start" << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("META-Start"));
						MIDI_data_pos = 0;
						META_len = -1;
						META_type = -1;
						in_META = true;
						running_status = 0;
					}
					else {
						#if LOG_EVENTS == 1
							logfile << "Error: Unknown MIDI Event " << (int)buf[pos] << "\n";
						#endif
						UE_LOG(LogTemp, Error, TEXT("Error: Unknown MIDI Event %u"), buf[pos]);
						running_status = 0;
					}
				}

				else {
					if (running_status <= 0x7F) {
						#if LOG_EVENTS == 1
							logfile << "Error: Unknown MIDI running status " << (int)running_status << "\n";
						#endif
						UE_LOG(LogTemp, Error, TEXT("Error: Unknown MIDI running status %u"), running_status);
					}
					else if (running_status <= 0x8F) {
						if (Note_to_stop_nr == -1) {
							Note_to_stop_nr = buf[pos];
						}
						else {
							//Note OFF Event
							//if (MTrk_nr >= 37 && MTrk_nr <= 38 || MTrk_nr <= 2) {
							#if LOG_NOTES == 1
								logfile << MTrk_nr << ", " << tick << ", Note_off_c, " << (int)(running_status - 0x80) << ", " << (int)Note_to_stop_nr << ", " << (int)buf[pos] << "\n";
							#endif
							//}
							stoppos[frame_nr][Note_to_stop_nr] += 1;
							Note_to_stop_nr = -1;
							in_deltatime = true;
						}
					}
					else if (running_status <= 0x9F) {
						if (Note_to_play_nr == -1) {
							Note_to_play_nr = buf[pos];
						}
						else {
							//if (MTrk_nr >= 37 && MTrk_nr <= 38 || MTrk_nr <= 2) {
							#if LOG_NOTES == 1
								logfile << MTrk_nr << ", " << tick << ", Note_on_c, " << (int)(running_status - 0x90) << ", " << (int)Note_to_play_nr << ", " << (int)buf[pos] << "\n";
							#endif
							//}
							if (buf[pos] > 0) {
								//Note ON Event
								spawnpos[frame_nr][Note_to_play_nr] += 1;
							}
							else {
								//Note OFF Event
								stoppos[frame_nr][Note_to_play_nr] += 1;
							}
							Note_to_play_nr = -1;
							in_deltatime = true;
						}
					}

					else if (running_status <= 0xAF) {
						if (Pressure_note == -1) {
							Pressure_note = buf[pos];
						}
						else {
							Pressure_value = buf[pos];
							Pressure_note = -1;
							in_deltatime = true;
						}
					}
					else if (running_status <= 0xBF) {
						if (Controller_type == -1) {
							Controller_type = buf[pos];
						}
						else {
							Controller_value = buf[pos];
							Controller_type = -1;
							#if LOG_EVENTS == 1
								logfile << "Controller-End!\n";
							#endif
							UE_LOG(LogTemp, Log, TEXT("Controller-End!"));
							in_deltatime = true;
						}
					}
					else if (running_status <= 0xCF) {
						#if LOG_EVENTS == 1
							logfile << "Program Change: Channel " << (int)(running_status - 0xC0) << " = " << (int)buf[pos] << "\n";
						#endif
						UE_LOG(LogTemp, Log, TEXT("Program Change: Channel %d = %u"), running_status - 0xC0, buf[pos]);
						Instument_table[running_status - 0xC0] = buf[pos];
						in_deltatime = true;
					}
					else if (running_status <= 0xDF) {
						Pressure_table[running_status - 0xD0] = buf[pos];
						in_deltatime = true;
					}
					else if (running_status <= 0xEF) {
						if (Pitch_lsb == -1) {
							Pitch_lsb = buf[pos];
						}
						else {
							Pitch_value = (buf[pos] << 7) + Pitch_lsb;
							Pitch_lsb = -1;
							in_deltatime = true;
						}
					}
					else if (in_SysEx == true) {
						if (SysEx_len == -1) {
							#if LOG_EVENTS == 1
								logfile << "SysEx_len = " << (int)buf[pos] << "\n";
							#endif
							UE_LOG(LogTemp, Log, TEXT("SysEx_len = %u"), buf[pos]);
							SysEx_len = buf[pos];
							MIDI_data_size = buf[pos];
							if (SysEx_len == 0) {
								goto SysEx_end_label;
							}
						}
						else {
							--SysEx_len;
							if (SysEx_len == 0) {
								SysEx_end_label:
								if (buf[pos] == 0xF7) {
									SysEx_escape_seq = true;
								}
								else {
									SysEx_escape_seq = false;
								}
								SysEx_len = -1;
								in_SysEx = false;
							}
						}
					}
					else {
						#if LOG_EVENTS == 1
							logfile << "Error: Unknown MIDI Data " << (int)buf[pos] << "\n";
						#endif
						UE_LOG(LogTemp, Error, TEXT("Error: Unknown MIDI Data %u"), buf[pos]);
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

	if (FrameNr > spawnpos.size() - 2) {
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, "Done!");
		++FrameNr;
		return;
	}

	uint8 notenr;
	uint32 spawnnr;
	uint32 stopnr;
	uint32 spawncount;
	uint32 stopcount;
	FVector location;
	FRotator rotate = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.Owner = this;
	UWorld* world = GetWorld();
	for (notenr = 0; notenr < 128; ++notenr) {
		spawnnr = spawnpos[FrameNr][notenr];
		stopnr = stoppos[FrameNr][notenr];
		if (spawnnr > spawnreduction) {
			spawnnr = spawnreduction;
		}
		if (stopnr > spawnreduction) {
			stopnr = spawnreduction;
		}
		if (spawnnr > 0) {
			for (spawncount = 0; spawncount < spawnnr; ++spawncount) {
				location = FVector((float)(90600.0f + spawncount * 100.0f), (float)(645000 - notenr * 100.0f), -110000.0f);
				SpawnInfo.Name = *FString::Printf(TEXT("F%uN%uC%u"), FrameNr, notenr, spawncount);
				blocks.push(world->SpawnActor<ABlock>(ABlock::StaticClass(), location, rotate, SpawnInfo));
#if MIDI_OUT == 1 && WITH_EDITOR == 0
				// Note On: 0x90, notenr, 0x64
				//UE_LOG(LogTemp, Log, TEXT("ON: %u"), notenr);
				message[0] = 0x90;
				message[1] = notenr;
				message[2] = 0x64;
				midiout->sendMessage(&message);
#endif
			}
		}
		if (stopnr > 0) {
			for (stopcount = 0; stopcount < stopnr; ++stopcount) {
#if MIDI_OUT == 1 && WITH_EDITOR == 0 
				// Note Off: 0x90, 64, 40
				//UE_LOG(LogTemp, Log, TEXT("OFF: %u"), notenr);
				//message[0] = 0x80;
				//message[1] = notenr;
				//message[2] = 0;
				//midiout->sendMessage(&message);
#endif
			}
		}
	}

	while (blocks.size() > blocklimit) {
		blocks.front()->Destroy();
		blocks.pop();
	}


	//++viewStats;
	//if (viewStats == 10) {
	//viewStats = 0;
	//TArray<AActor*> FoundActors;
	//FoundActors[0]->Destroy()
	//UGameplayStatics::GetAllActorsOfClass(world, ABlock::StaticClass(), FoundActors);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green,
		  "FPS: " + FString::SanitizeFloat(1.0f / DeltaTime)
		+ "\nBlocks: " + FString::FromInt(blocks.size())
		+ "\nFrame: " + FString::FromInt(FrameNr) + "/" + FString::FromInt(spawnpos.size())
		+ "\nLength: " + FString::FromInt(int(spawnpos.size()/3600.0f)) + ":" + FString::FromInt(int(spawnpos.size()/60.0f) % 60) 
		+ "\nPos: " + GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation().ToString());
	//}


	//ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(world, 0);
	//myCharacter->GetActorLocation();
	//myCharacter->SetActorRotation();

	//GetHighResScreenshotConfig().ResolutionMultiplier = 4; //Sets the res multiplier
	//GetWorld()->GetGameViewport()->Viewport->TakeHighResScreenShot(); //Sets the flag in the viewport to take the high-res shot.

	++FrameNr;

}

