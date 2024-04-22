// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Components/TimelineComponent.h"
#include "CharacterBase.generated.h"



/**
 * 
 */
UCLASS()
class SOILDERUP_API ACharacterBase : public APaperZDCharacter
{

	GENERATED_BODY()
public:
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ToogleSpriteFlicker();
	UFUNCTION()
	void StopSpriteFlicker();

	UFUNCTION()
	void HandleFlashEffectProgress(float Value);

	void TriggerSpriteFlashEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprite")
	float SpriteFlickerFreq=0.10f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprite")
	float StopFlickerTime=0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Health = 20.0f;

	UPROPERTY()
	FTimeline FlashEffectTimeline;
	UPROPERTY(EditDefaultsOnly, Category = "Timelines")
	UCurveFloat* FlashEffectFloatCurve;
	UPROPERTY(EditDefaultsOnly, Category="Timelines")
	float FlashEffectPlayRate = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprite")
	float FlashMultiplier = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprite")
	FVector4 FlashColor = FVector4(1, 1, 1, 1);




	FTimerHandle ToogleSpriteFlickerTimerHandle;
	FTimerHandle StopSpriteFlickerTimerHandle;

	UMaterialInstanceDynamic* SpriteLitMaterialWithFlash;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
