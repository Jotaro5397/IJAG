// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FieldPlayer.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    CacheAllPlayers();

    // Auto-possess first player
    if (AllPlayers.Num() > 0)
    {
        Possess(Cast<APawn>(AllPlayers[0]));
    }
}

void AMyPlayerController::SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime)
{
    if (!NewViewTarget) return;

    // Ensure controller has valid pawn
    if (GetPawn() && GetPawn() == NewViewTarget)
    {
        Super::SetViewTargetWithBlend(NewViewTarget, BlendTime);
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

    // Unpossess current pawn safely
    if (GetPawn())
    {
        UnPossess();
    }

    // Cycle players
    CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayers.Num();
    if (AFieldPlayer* NewPlayer = Cast<AFieldPlayer>(AllPlayers[CurrentPlayerIndex]))
    {
        // Critical fix: Possess before setting view target
        Possess(NewPlayer);
        SetViewTargetWithBlend(NewPlayer, 0.5f);

        // Alternative: Use camera component from pawn
        // if (NewPlayer->GetCameraComponent()) 
        // {
        //     SetViewTargetWithBlend(NewPlayer->GetCameraComponent()->GetOwner(), 0.5f);
        // }
    }
}

void AMyPlayerController::CacheAllPlayers()
{
    AllPlayers.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFieldPlayer::StaticClass(), AllPlayers);

    // Validate actors
    AllPlayers.RemoveAll([](AActor* Actor) {
        return !IsValid(Actor) || Cast<AFieldPlayer>(Actor) == nullptr;
        });
}
