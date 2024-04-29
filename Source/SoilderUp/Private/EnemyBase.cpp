// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameUtils.h"
#include "WeaponBase.h"
#include "PaperFlipbookComponent.h"

AEnemyBase::AEnemyBase() : ACharacterBase() {

	UCapsuleComponent* CapsuleComponentRef = GetCapsuleComponent();
	CapsuleComponentRef->SetCollisionProfileName(FName("Enemy"));
	CapsuleComponentRef->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnBeginOverlap);
	//Lock Rotation (Only works when Simulate Physics is On)
	CapsuleComponentRef->BodyInstance.bLockXRotation = true;
	CapsuleComponentRef->BodyInstance.bLockYRotation = true;
	CapsuleComponentRef->BodyInstance.bLockZRotation = true;
	CapsuleComponentRef->SetSimulatePhysics(false);

	UCharacterMovementComponent* MovementComponent= GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = MovementSpeed;
	MovementComponent->Velocity = FVector(0, MovementSpeed, 0);
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));


}

void AEnemyBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AddMovementInput(FVector(0.0f, 1.0f, 0.0f));
}

void AEnemyBase::BeginPlay() {
	Super::BeginPlay();


}

void AEnemyBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	AWeaponBase* Weapon = Cast<AWeaponBase>(OtherActor);
	if (Weapon) {
		Health = Health-Weapon->Damage;
		
		if (Health <= 0) {
			
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//GameUtils::LogMessage("Enemy Destroyed", FColor::Green);
			Destroy();
		}
		else {
			KnockBack();
			TriggerSpriteFlashEffect();
			//GetWorld()->GetTimerManager().SetTimer(ToogleSpriteFlickerTimerHandle, this, &ACharacterBase::ToogleSpriteFlicker, SpriteFlickerFreq, true);
			//GetWorld()->GetTimerManager().SetTimer(StopSpriteFlickerTimerHandle, this, &ACharacterBase::StopSpriteFlicker, StopFlickerTime, false);		
		}
		Weapon->HandleDeath();
	}

}

void AEnemyBase::KnockBack() {
	UPrimitiveComponent* PhysicsComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	if (PhysicsComponent)
	{
		UCapsuleComponent* CapsuleComponentRef = GetCapsuleComponent();
		CapsuleComponentRef->SetSimulatePhysics(true);
		PhysicsComponent->AddImpulse(FVector(0,-1,0) * KnockbackStrength, NAME_None, true);

		GetWorld()->GetTimerManager().SetTimer(DisablePhysicsTimerHandle, this, &AEnemyBase::DisablePhysics, 0.2f, false);
	}

}

void AEnemyBase::DisablePhysics() {
	UCapsuleComponent* CapsuleComponentRef = GetCapsuleComponent();
	CapsuleComponentRef->SetSimulatePhysics(false);
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}