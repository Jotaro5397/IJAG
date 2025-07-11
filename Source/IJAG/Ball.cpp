// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
    PrimaryActorTick.bCanEverTick = true;

    BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollision"));
    RootComponent = BallCollision;
    BallCollision->InitSphereRadius(24.0f);
    BallCollision->SetSimulatePhysics(true);
    BallCollision->SetCollisionProfileName(TEXT("PhysicsActor"));

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(BallCollision);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BallMesh->SetSimulatePhysics(false); // Only BallCollision simulates
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  
}