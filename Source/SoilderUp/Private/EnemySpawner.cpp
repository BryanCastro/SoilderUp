// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "EnemyBase.h"
#include "GameUtils.h"
// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

    RootComponent = SceneComponent;

    // Example Blueprint path - replace with your actual Blueprint path
    static ConstructorHelpers::FClassFinder<AEnemyBase> EnemyBP(TEXT("/Game/Blueprints/BP_EnemyBase.BP_EnemyBase_C"));
    if (EnemyBP.Succeeded())
    {
        EnemyBlueprint = EnemyBP.Class;
    }
    else {
        GameUtils::LogMessage("EnemySpawner.cpp: Failed to Find BP_EnemyBase");
    }




}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();


    WorldRef = GetWorld();


    SetupSpawnLocations();
    GetWorldTimerManager().SetTimer(SpawnEnemyTimerHandle, this, &AEnemySpawner::SpawnEnemy, EnemySpawnTime, true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SetupSpawnLocations() {

    // Clear existing locations if needed
    for (USceneComponent* Component : SpawnLocations)
    {
        Component->DestroyComponent();
    }
    SpawnLocations.Empty();

    for (int i = 0; i < SpawnNum; i++) {
        // Create a new Scene Component for each spawn location
        USceneComponent* SpawnLocation = NewObject<USceneComponent>(this, USceneComponent::StaticClass());
        if (SpawnLocation)
        {
            SpawnLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            SpawnLocation->SetRelativeLocation(FVector(LocationXStart, 0.0f, 0.0f)); // Example offset
            SpawnLocation->RegisterComponent(); // Important to register the component
            SpawnLocations.Add(SpawnLocation);
            LocationXStart += 100;
        }
    }
}

void AEnemySpawner::SpawnEnemy() {
    if(SpawnLocations.Num() == 0)
    {
        GameUtils::LogMessage("EnemySpawner.cpp: Empty Spawn Locations Array");
        return;
    }

    int32 Index = FMath::RandRange(0, SpawnLocations.Num() - 1);
    USceneComponent* SceneComp= SpawnLocations[Index]; // Return the random element

    FTransform SpawnTransform = SceneComp->GetComponentTransform();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = NULL;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    AEnemyBase* SpawnedEnemy = WorldRef->SpawnActor<AEnemyBase>(EnemyBlueprint, SpawnTransform, SpawnParams);
    if (SpawnedEnemy) {
        SpawnedEnemy->SetActorHiddenInGame(false);  // Make sure the actor is not hidden
      //  GameUtils::LogMessage("Successfully Spawned Actor", FColor::Green);
    }
    else {
        GameUtils::LogMessage("Failed to Spawn Enemy");
    }


}