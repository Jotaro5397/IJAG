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

    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
    SelectionDecal->SetupAttachment(RootComponent);
    SelectionDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f)); // Face upwards
    SelectionDecal->DecalSize = FVector(100.f, 200.f, 200.f);
    SelectionDecal->SetVisibility(false);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(TEXT("/Game/Path/To/DefaultDecalMaterial"));
    if (DefaultMaterial.Succeeded())
    {
        SelectionDecal->SetDecalMaterial(DefaultMaterial.Object);
    }
}
void AFieldPlayer::BeginPlay()
{
    Super::BeginPlay();


    if (PossessedDecalMaterial)
    {
        UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(PossessedDecalMaterial, this);
        SelectionDecal->SetDecalMaterial(DynMaterial);
    }

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
    if (SelectionDecal)
    {
        SelectionDecal->SetVisibility(bIsPossessed);
        // ... (material logic)
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
    if (!Controller) return;

    // Update LastKnownRotation ONLY when controlled
    LastKnownRotation = Controller->GetControlRotation();

    // Movement direction based on LastKnownRotation
    const FRotator YawRotation(0, LastKnownRotation.Yaw, 0);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, AxisValue);
}

void AFieldPlayer::MoveRight(float AxisValue)
{
    if (!Controller) return;

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