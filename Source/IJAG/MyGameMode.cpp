 // Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "BroadCamera.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "FieldPlayer.h"

AMyGameMode::AMyGameMode()
{
    // Disable default pawn (use camera as primary view)
    DefaultPawnClass = nullptr;
    PlayerControllerClass = AMyPlayerController::StaticClass();
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    ABroadCamera* CameraActor = Cast<ABroadCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass()));
    ABall* BallActor = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

    if (CameraActor && BallActor)
    {
        CameraActor->SetTargetBall(BallActor);
    }

    InitializeCameraAndBall();
}

void AMyGameMode::InitializeCameraAndBall()
{
    // Spawn or find the ball (using ABall)
    ABall* BallActor = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
    if (!BallActor && BallClass)
    {
        FActorSpawnParameters BallSpawnParams;
        BallActor = GetWorld()->SpawnActor<ABall>(BallClass, FVector::ZeroVector, FRotator::ZeroRotator, BallSpawnParams);
    }

}
