// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyAllBarrier.h"
#include "Components/BoxComponent.h"
#include "EnemyBase.h"

// Sets default values
ADestroyAllBarrier::ADestroyAllBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	RootComponent = BoxComponent;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestroyAllBarrier::OnBeginOverlap);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ADestroyAllBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestroyAllBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyAllBarrier::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);

	if (Enemy) {
		Enemy->Destroy();
	}

}