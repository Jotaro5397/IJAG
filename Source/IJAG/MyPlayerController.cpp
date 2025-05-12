// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BroadCamera.h"
#include "FieldPlayer.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 1. Find and possess the FieldPlayer pawn
    APawn* PlayerPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AFieldPlayer::StaticClass()));
    if (PlayerPawn)
    {
        Possess(PlayerPawn); // Take control of the pawn
    }

    // 2. Set camera view
    ABroadCamera* CameraActor = Cast<ABroadCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass()));
    if (CameraActor)
    {
        SetViewTargetWithBlend(CameraActor, 0.0f); // Instant switch
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("SwitchPlayer", IE_Pressed, this, &AMyPlayerController::SwitchPlayer);
}

void AMyPlayerController::SwitchPlayer()
{
    if (AllPlayers.Num() == 0) CacheAllPlayers();
    if (AllPlayers.Num() < 2) return;

    // Unpossess current pawn (no need for LosePossession)
    if (GetPawn())
    {
        UnPossess(); // Native unpossession
    }

    // Cycle through players
    CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayers.Num();
    if (AFieldPlayer* NewPlayer = Cast<AFieldPlayer>(AllPlayers[CurrentPlayerIndex]))
    {
        Possess(NewPlayer); // Native possession
        SetViewTargetWithBlend(NewPlayer, 0.5f);
    }
}

void AMyPlayerController::CacheAllPlayers()
{
    AllPlayers.Empty();
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AFieldPlayer::StaticClass(),
        AllPlayers
    );
}