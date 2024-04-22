// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"



/**
 * 
 */
UCLASS()
class SOILDERUP_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void KnockBack();
	void DisablePhysics();

	FTimerHandle DisablePhysicsTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	float MovementSpeed = 500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float KnockbackStrength = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Damage = 1.0f;


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
