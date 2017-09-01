// Fill out your copyright notice in the Description page of Project Settings.

#include "GDC_Demo.h"
#include "LoadingScreenGameInstance.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"

void ULoadingScreenGameInstance::Init()
{
	UGameInstance::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingScreenGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMap.AddUObject(this, &ULoadingScreenGameInstance::EndLoadingScreen);
}

void ULoadingScreenGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.bMoviesAreSkippable = false;
		//LoadingScreen.MinimumLoadingScreenDisplayTime = 10;
		LoadingScreen.MoviePaths.Add("CoC_Hack_720");
		//LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		GetMoviePlayer()->PlayMovie();
	}
}

void ULoadingScreenGameInstance::EndLoadingScreen()
{

}


