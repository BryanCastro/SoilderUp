// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
struct FTimerHandle;

UCLASS()
class SOILDERUP_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


	FTimerHandle WeaponDeathTimerHandle;
	void OnDeath();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement");
	float MovementSpeed=500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement");
	float LifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats");
	float Damage = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
