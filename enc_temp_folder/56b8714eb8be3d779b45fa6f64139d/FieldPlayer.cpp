// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

AFieldPlayer::AFieldPlayer()
{
    PrimaryActorTick.bCanEverTick = false;
    bUseControllerRotationYaw = false;

    // Enable orientation towards movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // Rotation speed
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void AFieldPlayer::BeginPlay()
{
    Super::BeginPlay();

    // Auto-possess if no controller
    if (!GetController())
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->Possess(this); // Take control of this actor
        }
    }

    // Initialize animation instance
    AnimInstance = Cast<UFieldPlayerAnim>(GetMesh()->GetAnimInstance());
}

void AFieldPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AFieldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AFieldPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFieldPlayer::MoveRight);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFieldPlayer::Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFieldPlayer::StopSprinting);
}

void AFieldPlayer::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector(), AxisValue);
}

void AFieldPlayer::MoveRight(float AxisValue)
{
    AddMovementInput(GetActorRightVector(), AxisValue);
}

void AFieldPlayer::Sprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintSpeedMultiplier;
}

void AFieldPlayer::StopSprinting()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AFieldPlayer::TriggerPassAnimation()
{
    if (AnimInstance)
    {
        AnimInstance->StartPassAnimation();
        GetWorldTimerManager().SetTimer(
            PassAnimationTimer,
            this,
            &AFieldPlayer::OnPassAnimationCompleted,
            0.5f,
            false
        );
    }
}

void AFieldPlayer::OnPassAnimationCompleted()
{
    if (AnimInstance)
    {
        AnimInstance->StopPassAnimation();
    }
}