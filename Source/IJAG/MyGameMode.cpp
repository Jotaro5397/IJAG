 // Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "BroadCamera.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"

AMyGameMode::AMyGameMode()
{
    // Disable default pawn (use camera as primary view)
    DefaultPawnClass = nullptr;
    PlayerControllerClass = AMyPlayerController::StaticClass();
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
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

    // Spawn or find the camera
    ABroadCamera* CameraActor = Cast<ABroadCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass()));
    if (!CameraActor && BroadcastCameraClass)
    {
        FActorSpawnParameters CameraSpawnParams;
        CameraActor = GetWorld()->SpawnActor<ABroadCamera>(BroadcastCameraClass, FVector::ZeroVector, FRotator::ZeroRotator, CameraSpawnParams);
    }

    // Link camera to ball
    if (CameraActor && BallActor)
    {
        CameraActor->SetTargetBall(BallActor);

        // Force camera view
        AMyPlayerController* PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (PC)
        {
            PC->SetViewTargetWithBlend(CameraActor, 0.0f);
        }
    }
}
