// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameUtils.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "WeaponBase.h"
#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"



APlayerBase::APlayerBase() : ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	WeaponSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSpawnLocation"));

	Camera->SetupAttachment(SpringArm);
	SpringArm->SetupAttachment(RootComponent);
	WeaponSpawnLocation->SetupAttachment(RootComponent);

	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 3000.0f;
	FRotator InitialRotation = SpringArm->GetComponentRotation();
	InitialRotation.Yaw += 90.0f,
	InitialRotation.Pitch += 30.0f;

	SpringArm->SetWorldRotation(InitialRotation);

	UCapsuleComponent* CapsuleComponentRef = GetCapsuleComponent();
	CapsuleComponentRef->SetCollisionProfileName(FName("Player"));
	CapsuleComponentRef->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::OnBeginOverlap);



	// Example Blueprint path - replace with your actual Blueprint path
	static ConstructorHelpers::FClassFinder<AWeaponBase> WeaponBP(TEXT("/Game/Blueprints/BP_WeaponBase.BP_WeaponBase_C"));
	if (WeaponBP.Succeeded())
	{
		WeaponBlueprint = WeaponBP.Class;
	}

	WorldRef = GetWorld();
}

void APlayerBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerControllerRef = Cast<APlayerController>(GetController());
	if (PlayerControllerRef) {
		UEnhancedInputComponent* PlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerControllerRef->InputComponent);
		if (PlayerInputComponent) {
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerControllerRef->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (InputSubsystem && IMC_Player)
			{
				InputSubsystem->AddMappingContext(IMC_Player, 0); // 0 is the priority
				GameUtils::LogMessage("PlayerBase.cpp: Successfully setup Enhanced Input", FColor::Green);
			}
		}

		PlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerBase::Move);
	}


}

void APlayerBase::Move(const FInputActionValue& Value) {
	FVector2D InputVector = Value.Get<FVector2D>();

	//GameUtils::LogMessage(FString::Printf(TEXT("Movement: %s"), *InputVector.ToString()));

	AddMovementInput(FVector(1.0f, 0, 0), InputVector.X);

	FVector Location = GetActorLocation();
	//GameUtils::LogMessage(FString::Printf(TEXT("Actor Location: %s"), *Location.ToString()));
}

void APlayerBase::BeginPlay() {

	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnWeaponTimerHandle, this, &APlayerBase::SpawnWeapon, WeaponSpawnTime, true);
}

void APlayerBase::SpawnWeapon() {
	FTransform SpawnTransform = WeaponSpawnLocation->GetComponentTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NULL;
	SpawnParams.Instigator = GetInstigator();

	AWeaponBase* SpawnedWeapon=WorldRef->SpawnActor<AWeaponBase>(WeaponBlueprint, SpawnTransform, SpawnParams);
	if (SpawnedWeapon) {
		SpawnedWeapon->SetActorHiddenInGame(false);  // Make sure the actor is not hidden
		//GameUtils::LogMessage("Successfully Spawned Actor", FColor::Green);
	}
	else {
		GameUtils::LogMessage("Failed to Spawn Weapon");
	}
}

void APlayerBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
	if (Enemy) {
		if (PlayerHitSound)
			UGameplayStatics::PlaySoundAtLocation(this, PlayerHitSound, GetActorLocation());
		else
			GameUtils::LogMessage("PlayerBase.cpp: Failed to Play PlayerHitSound");

		Health = Health - Enemy->Damage;

		//GameUtils::LogMessage(FString::Printf(TEXT("Enemy collided with: %s"), *Enemy->GetName()));
		//GameUtils::LogMessage(FString::Printf(TEXT("Health: %f"), Health));


		if (Health <= 0) {
			//GameUtils::LogMessage("Enemy Destroyed", FColor::Green);
			Destroy();
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(ToogleSpriteFlickerTimerHandle, this, &ACharacterBase::ToogleSpriteFlicker, SpriteFlickerFreq, true);
			GetWorld()->GetTimerManager().SetTimer(StopSpriteFlickerTimerHandle, this, &ACharacterBase::StopSpriteFlicker, StopFlickerTime, false);
		}
	}

}

