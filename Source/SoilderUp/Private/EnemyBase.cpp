// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameUtils.h"
#include "WeaponBase.h"

AEnemyBase::AEnemyBase() : ACharacterBase() {

	UCapsuleComponent* CapsuleComponentRef = GetCapsuleComponent();
	CapsuleComponentRef->SetCollisionProfileName(FName("Enemy"));
	CapsuleComponentRef->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnBeginOverlap);

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

void AEnemyBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	AWeaponBase* Weapon = Cast<AWeaponBase>(OtherActor);
	if (Weapon) {
		Health = Health-Weapon->Damage;
		
		if (Health <= 0) {
			//GameUtils::LogMessage("Enemy Destroyed", FColor::Green);
			OtherActor->Destroy();
			Destroy();
		}
	}

}