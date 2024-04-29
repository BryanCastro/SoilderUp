// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameUtils.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	SphereComponent->SetupAttachment(RootComponent);
	MeshComponent->SetupAttachment(SphereComponent);
	ProjectileMovementComp->UpdatedComponent = RootComponent;

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->MaxSpeed = MovementSpeed;
	ProjectileMovementComp->Velocity = FVector(0.0f, -MovementSpeed, 0.0f);

	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore); //Player
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); //Enemy

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(WeaponDeathTimerHandle, this, &AWeaponBase::DestroyWeapon, LifeTime, false);
	
	if (WeaponUsedSound) {
		UGameplayStatics::PlaySoundAtLocation(this, WeaponUsedSound, GetActorLocation());
	}
	else
		GameUtils::LogMessage("WeaponBase.cpp: Failed to Load WeaponUsed Sound");

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::HandleDeath() {
	if (WeaponHitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, WeaponHitSound, GetActorLocation());
		Destroy();
	}
	else {
		GameUtils::LogMessage("WeaponBase.cpp: Failed to Play WeaponDeathSound");
	}

}

void AWeaponBase::DestroyWeapon() {
	GameUtils::LogMessage("Audio Finished!");
	Destroy();
}

void AWeaponBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AWeaponBase, MovementSpeed))
	{
		ProjectileMovementComp->MaxSpeed = MovementSpeed;
		ProjectileMovementComp->Velocity = FVector(0.0f, -MovementSpeed, 0.0f);
	}
}