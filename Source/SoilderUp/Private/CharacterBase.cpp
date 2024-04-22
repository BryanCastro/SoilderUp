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

void ACharacterBase::BeginPlay() {
	Super::BeginPlay();

	if (FlashEffectFloatCurve) {

		//GameUtils::LogMessage("Found Curve");
		FOnTimelineFloat FlashEffectProgressFunction;
		FlashEffectProgressFunction.BindUFunction(this, FName("HandleFlashEffectProgress"));

		FlashEffectTimeline.AddInterpFloat(FlashEffectFloatCurve, FlashEffectProgressFunction);
		FlashEffectTimeline.SetLooping(false);
		FlashEffectTimeline.SetPlayRate(FlashEffectPlayRate);
	}

	SpriteLitMaterialWithFlash = GetSprite()->CreateDynamicMaterialInstance(0);
}

void ACharacterBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (FlashEffectTimeline.IsPlaying())
	{
		FlashEffectTimeline.TickTimeline(DeltaTime);
	}
}

void ACharacterBase::ToogleSpriteFlicker()
{
	UPaperFlipbookComponent* SpriteRef = GetSprite();
	bool NotVisibility = !SpriteRef->IsVisible();
	SpriteRef->SetVisibility(NotVisibility);

}

void ACharacterBase::TriggerSpriteFlashEffect() {

	SpriteLitMaterialWithFlash->SetScalarParameterValue("FlashMultiplier", FlashMultiplier);
	SpriteLitMaterialWithFlash->SetVectorParameterValue("FlashColor", FlashColor);

	FlashEffectTimeline.PlayFromStart();
}

void ACharacterBase::StopSpriteFlicker() {
	GetWorld()->GetTimerManager().ClearTimer(ToogleSpriteFlickerTimerHandle);
}

UFUNCTION()
void ACharacterBase::HandleFlashEffectProgress(float Value) {
	
	//GameUtils::LogMessage(FString::Printf(TEXT("Flash Value: %f"), Value));
	SpriteLitMaterialWithFlash->SetScalarParameterValue("FlashMultiplier", Value);
}
