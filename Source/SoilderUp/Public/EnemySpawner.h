// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;


UCLASS()
class SOILDERUP_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	USceneComponent* SceneComponent;

	FTimerHandle SpawnEnemyTimerHandle;

	void SetupSpawnLocations();
	void SpawnEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	int32 SpawnNum=6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	float LocationXStart = -300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	float LocationXOffset = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	float EnemySpawnTime=0.1f;

	UPROPERTY(EditAnywhere, Category = "Enemies")
	TSubclassOf<AEnemyBase> EnemyBlueprint;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	TArray<USceneComponent*> SpawnLocations;
	UWorld* WorldRef;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
