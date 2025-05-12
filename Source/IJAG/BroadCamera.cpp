// Fill out your copyright notice in the Description page of Project Settings.


#include "BroadCamera.h"
#include "Kismet/GameplayStatics.h"

ABroadCamera::ABroadCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->TargetArmLength = 1000.0f;
    RootComponent = SpringArmComponent;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetActive(true);
}

void ABroadCamera::BeginPlay()
{
    Super::BeginPlay();

    // Optional: Auto-find ball if needed
    // TargetBall = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
}

void ABroadCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetBall)
    {
        FVector BallLocation = TargetBall->GetActorLocation();
        FVector TargetLocation = BallLocation + CameraOffset;
        FVector CurrentLocation = GetActorLocation();

        FVector NewLocation = FMath::VInterpTo(
            CurrentLocation,
            TargetLocation,
            DeltaTime,
            InterpSpeed
        );

        SetActorLocation(NewLocation);
    }
}

void ABroadCamera::SetTargetBall(ABall* NewTargetBall)
{
    TargetBall = NewTargetBall;
}