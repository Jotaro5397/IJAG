// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FieldPlayer.h"
#include "BroadCamera.h"
#include "Ball.h"


AMyPlayerController::AMyPlayerController() 
{
    bAutoManageActiveCameraTarget = false; // Default is true, but enforce it
}


void AMyPlayerController::BeginPlay() {
    Super::BeginPlay();

    SetBroadCameraAsViewTarget();

    // Get a reference to the existing character in the scene
    AFieldPlayer* PlayerCharacter = Cast<AFieldPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFieldPlayer::StaticClass()));
    if (PlayerCharacter) 
    {
        Possess(PlayerCharacter);
    }

    ABroadCamera* BroadCamera = Cast<ABroadCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass()));
    if (BroadCamera)
    {
        SetViewTarget(BroadCamera);
    }

    
    ABroadCamera* LevelCamera = Cast<ABroadCamera>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass())
    );

    // Set it as the active view target
    if (LevelCamera) {
        SetViewTarget(LevelCamera);
    }

    // Auto-possess the first player
    CacheAllPlayers();
    if (AllPlayers.Num() > 0) {
        PossessPlayerAndSetView();
    }
}

void AMyPlayerController::SetBroadCameraAsViewTarget()
{
    ABroadCamera* BroadCamera = Cast<ABroadCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass()));
    if (BroadCamera)
    {
        SetViewTarget(BroadCamera); // Switch immediately without blending
    }
}

void AMyPlayerController::SwitchPlayer()
{
    // Refresh the player list
    CacheAllPlayers();

    if (AllPlayers.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough players!"));
        return;
    }

    // Unpossess current pawn
    if (APawn* CurrentPawn = GetPawn())
    {
        UnPossess();
        CurrentPawn->DisableInput(this);
    }

    // Cycle players
    CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayers.Num();
    UE_LOG(LogTemp, Log, TEXT("Switching to index: %d"), CurrentPlayerIndex);

    // Possess the new player and update the camera
    PossessPlayerAndSetView();
}

void AMyPlayerController::CacheAllPlayers()
{
    AllPlayers.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFieldPlayer::StaticClass(), AllPlayers);

    // Debug: Print all found players
    for (AActor* Actor : AllPlayers)
    {
        UE_LOG(LogTemp, Log, TEXT("Found player: %s"), *Actor->GetName());
    }

    // Remove invalid actors
    AllPlayers.RemoveAll([](AActor* Actor) {
        bool bIsValid = IsValid(Actor) && Actor->IsA<AFieldPlayer>();
        if (!bIsValid) UE_LOG(LogTemp, Warning, TEXT("Removed invalid actor"));
        return !bIsValid;
        });

    UE_LOG(LogTemp, Log, TEXT("Total players: %d"), AllPlayers.Num());
}


void AMyPlayerController::PossessPlayerAndSetView() {
    if (AFieldPlayer* NewPlayer = Cast<AFieldPlayer>(AllPlayers[CurrentPlayerIndex])) {
        Possess(NewPlayer);
        NewPlayer->EnableInput(this);
        
    }
}

void AMyPlayerController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
   
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind "SwitchPlayer" action to Shift key
    InputComponent->BindAction("SwitchPlayer", IE_Pressed, this, &AMyPlayerController::SwitchPlayer);
}