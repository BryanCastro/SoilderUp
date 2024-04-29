// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"


class USoundBase;
class UAudioComponent;
/**
 * 
 */
UCLASS()
class SOILDERUP_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	UAudioComponent* AudioComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sounds and Music");
	USoundBase* Music;
	
private:
	virtual void BeginPlay() override;

};
