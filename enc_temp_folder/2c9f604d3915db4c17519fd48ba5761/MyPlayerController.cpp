// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "FieldPlayer.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("SwitchPlayer", IE_Pressed, this, &AMyPlayerController::SwitchPlayer);
}

void AMyPlayerController::SwitchPlayer()
{
    if (AllPlayers.Num() == 0) CacheAllPlayers();
    if (AllPlayers.Num() < 2) return;

    // Unpossess current
    if (GetPawn())
    {
        if (AFieldPlayer* CurrentPlayer = Cast<AFieldPlayer>(GetPawn()))
        {
            CurrentPlayer->LosePossession();
        }
        UnPossess();
    }

    // Cycle through players
    CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayers.Num();
    if (AFieldPlayer* NewPlayer = Cast<AFieldPlayer>(AllPlayers[CurrentPlayerIndex]))
    {
        Possess(NewPlayer);
        NewPlayer->BecomePossessed(this);
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