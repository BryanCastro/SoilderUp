// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameUtils.h"

ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookFinder(TEXT("PaperFlipbook'/Game/PaperAssets/Player/UpWalk.UpWalk'"));


	if (FlipbookFinder.Succeeded()) {
		GetSprite()->SetFlipbook(FlipbookFinder.Object);
		//GameUtils::LogMessage("CharacterBase.cpp: Successfully loaded flipbook asset", FColor::Green);
	}
	else {
		GameUtils::LogMessage("CharacterBase.cpp: Failed to load flipbook asset");

	}

	GetSprite()->CastShadow = true;
} 
