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

   
    // Set BroadCamera as the initial view target
    SetBroadCameraAsViewTarget();

    // Auto-possess the first valid player (centralized logic)
    CacheAllPlayers();
    if (AllPlayers.Num() > 0) {
        PossessPlayerAndSetView();
    }
}

void AMyPlayerController::SetBroadCameraAsViewTarget() {
    ABroadCamera* BroadCamera = Cast<ABroadCamera>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ABroadCamera::StaticClass())
    );
    if (BroadCamera) {
        SetViewTarget(BroadCamera);
        // Sync controller rotation with the camera's orientation
        SetControlRotation(BroadCamera->GetActorRotation());
    }
}

void AMyPlayerController::SwitchPlayer() 
{
    CacheAllPlayers();

    if (AllPlayers.Num() < 2) {
        UE_LOG(LogTemp, Warning, TEXT("Not enough players!"));
        return;
    }

    // Unpossess current pawn
    if (APawn* CurrentPawn = GetPawn()) {
        CurrentPawn->DisableInput(this);
        UnPossess();
    }

    // Update index safely
    CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayers.Num();
    PossessPlayerAndSetView();
}

void AMyPlayerController::CacheAllPlayers() {
    AllPlayers.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFieldPlayer::StaticClass(), AllPlayers);

    // Remove invalid actors
    AllPlayers.RemoveAll([](AActor* Actor) {
        return !IsValid(Actor) || !Actor->IsA<AFieldPlayer>();
        });

    // Reset index if the list is empty or invalid
    if (AllPlayers.Num() == 0) {
        CurrentPlayerIndex = 0;
        UE_LOG(LogTemp, Warning, TEXT("No valid players found!"));
        return;
    }

    // Ensure CurrentPlayerIndex is within bounds
    CurrentPlayerIndex = FMath::Clamp(CurrentPlayerIndex, 0, AllPlayers.Num() - 1);
}


void AMyPlayerController::PossessPlayerAndSetView()
{
    // Validate the player index first
    if (!AllPlayers.IsValidIndex(CurrentPlayerIndex)) {
        UE_LOG(LogTemp, Error, TEXT("Invalid player index!"));
        return;
    }

    // Retrieve the target player actor
    AActor* PlayerActor = AllPlayers[CurrentPlayerIndex];
    if (!IsValid(PlayerActor)) {
        UE_LOG(LogTemp, Error, TEXT("Player actor is invalid!"));
        return;
    }

    // Cast to AFieldPlayer (THIS DECLARES NewPlayer)
    AFieldPlayer* NewPlayer = Cast<AFieldPlayer>(PlayerActor);
    if (!IsValid(NewPlayer)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to AFieldPlayer!"));
        return;
    }

    // Capture outgoing player's rotation
    AFieldPlayer* OutgoingPlayer = Cast<AFieldPlayer>(GetPawn());
    if (OutgoingPlayer && OutgoingPlayer->IsPlayerControlled()) // <-- Add IsPlayerControlled check
    {
        OutgoingPlayer->LastKnownRotation = OutgoingPlayer->GetActorRotation();
    }

    // Unpossess and disable old pawn
    if (APawn* CurrentPawn = GetPawn())
    {
        CurrentPawn->DisableInput(this);
        UnPossess();
    }

    // Possess the new pawn
    Possess(NewPlayer);
    NewPlayer->EnableInput(this);

    // SYNC CONTROLLER TO LAST KNOWN ROTATION (NOT ACTOR ROTATION)
    SetControlRotation(NewPlayer->LastKnownRotation); // <-- Critical change

    NewPlayer->PossessedBy(this);

    // Debug log
    UE_LOG(LogTemp, Warning, TEXT("Possessed: %s"), *NewPlayer->GetName());

    // Debug log
    UE_LOG(LogTemp, Warning, TEXT("Possessed: %s"), *NewPlayer->GetName());
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn); // Call parent class logic

    // Optional: Add custom logic when possessing a pawn
    if (InPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Possessed pawn: %s"), *InPawn->GetName());
    }
}

void AMyPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    // Bind "SwitchPlayer" to Q (CONTROLLER INPUT)
    InputComponent->BindAction("SwitchPlayer", IE_Pressed, this, &AMyPlayerController::SwitchPlayer);
}