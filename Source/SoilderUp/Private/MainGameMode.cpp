// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "GameUtils.h"

AMainGameMode::AMainGameMode() {
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false; // Prevent it from playing the sound immediately
	AudioComponent->SetupAttachment(RootComponent);
}

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();

	if (Music) {
		AudioComponent->SetSound(Music);
		AudioComponent->Play();
	}
	else {
		GameUtils::LogMessage("MainGameMode.cpp: Failed to load Music");
	}
}