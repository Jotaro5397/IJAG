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

// MyGameMode.cpp
void AMyGameMode::InitializeCameraAndBall() {
    // Spawn BroadCamera if not placed in the level
    ABroadCamera* CameraActor = Cast<ABroadCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass()));
    if (!CameraActor && BroadcastCameraClass) {
        CameraActor = GetWorld()->SpawnActor<ABroadCamera>(BroadcastCameraClass);
    }

   
    // Set the camera as the view target
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && CameraActor) {
        PC->SetViewTarget(CameraActor);
    }
}
