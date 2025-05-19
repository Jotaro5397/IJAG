// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldPlayer.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

AFieldPlayer::AFieldPlayer()
{
    PrimaryActorTick.bCanEverTick = false;
    bUseControllerRotationYaw = false;

    LastKnownRotation = FRotator::ZeroRotator;

    // Enable orientation towards movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // Rotation speed
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionIndicator"));
    SelectionIndicator->SetupAttachment(RootComponent);

    // Adjust position, rotation, and scale
    SelectionIndicator->SetRelativeLocation(FVector(0.f, 0.f, 30.f)); // Raise above ground
    SelectionIndicator->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // Rotate to face upward
    SelectionIndicator->SetRelativeScale3D(FVector(2.f)); // Scale to make visible
    SelectionIndicator->SetVisibility(false);

    SelectionIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Make the material unable to collide

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane"));
    if (PlaneMesh.Succeeded())
    {
        SelectionIndicator->SetStaticMesh(PlaneMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> IndicatorMaterial(TEXT("/Game/Path/To/M_Circle"));
    if (IndicatorMaterial.Succeeded())
    {
        SelectionIndicator->SetMaterial(0, IndicatorMaterial.Object);
    }
}
void AFieldPlayer::BeginPlay()
{
    Super::BeginPlay();

    LastKnownRotation = GetActorRotation();

    

    // Initialize animation instance
    AnimInstance = Cast<UFieldPlayerAnim>(GetMesh()->GetAnimInstance());
}

void AFieldPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsPlayerControlled()) return;

}

// CORRECTED FUNCTION NAME
void AFieldPlayer::UpdateDecalVisibility(bool bIsPossessed)
{
    if (SelectionIndicator) // Changed from SelectionDecal
    {
        SelectionIndicator->SetVisibility(bIsPossessed);
    }
}

void AFieldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Pawn-specific inputs (WASD, Sprint)
    PlayerInputComponent->BindAxis("MoveForward", this, &AFieldPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFieldPlayer::MoveRight);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFieldPlayer::Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFieldPlayer::StopSprinting);
}

void AFieldPlayer::MoveForward(float AxisValue)
{
    if (!IsPlayerControlled() || !Controller) return;

    // Update LastKnownRotation ONLY when controlled
    LastKnownRotation = Controller->GetControlRotation();

    // Movement direction based on LastKnownRotation
    const FRotator YawRotation(0, LastKnownRotation.Yaw, 0);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, AxisValue);
}

void AFieldPlayer::MoveRight(float AxisValue)
{
    if (!IsPlayerControlled() || !Controller) return;

    // Update LastKnownRotation ONLY when controlled
    LastKnownRotation = Controller->GetControlRotation();

    // Movement direction based on LastKnownRotation
    const FRotator YawRotation(0, LastKnownRotation.Yaw, 0);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, AxisValue);
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