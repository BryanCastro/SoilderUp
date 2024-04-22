// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerBase.generated.h"


class UInputMappingContext;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class AWeaponBase;
struct FInputActionValue;
struct FTimerHandle;

/**
 * 
 */
UCLASS()
class SOILDERUP_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerBase();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle SpawnWeaponTimerHandle;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	void Move(const FInputActionValue& Value);
	void SpawnWeapon();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Miscellaneous")
	USceneComponent* WeaponSpawnLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_Player;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float WeaponSpawnTime = 0.25f;

	UWorld* WorldRef;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<AWeaponBase> WeaponBlueprint;

private:
	APlayerController* PlayerControllerRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};