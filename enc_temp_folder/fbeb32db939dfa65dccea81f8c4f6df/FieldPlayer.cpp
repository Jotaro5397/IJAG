// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Ball.h" 
#include "Components/CapsuleComponent.h"

AFieldPlayer::AFieldPlayer()
{
    PrimaryActorTick.bCanEverTick = false;
    bUseControllerRotationYaw = false;

    LastKnownRotation = FRotator::ZeroRotator;

    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFieldPlayer::OnBallHit);

    // Enable orientation towards movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // Rotation speed
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionIndicator"));
    SelectionIndicator->SetupAttachment(RootComponent);
    SelectionIndicator->SetRelativeLocation(IndicatorOffset); 
    SelectionIndicator->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
    SelectionIndicator->SetRelativeScale3D(IndicatorScale); 
    SelectionIndicator->SetVisibility(false);
    SelectionIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

    if (AttachedBall && IsPlayerControlled())
    {
        const FVector TargetLocation = GetActorLocation() +
            GetActorForwardVector() * BallDistance;

        // Calculate distance to target
        const float CurrentDistance = FVector::Distance(
            TargetLocation,
            AttachedBall->GetActorLocation()
        );

        // Only apply force if beyond tolerance
        if (CurrentDistance > PositionTolerance)
        {
            FVector ForceDirection = (TargetLocation - AttachedBall->GetActorLocation());
            ForceDirection.Normalize();

            AttachedBall->GetBallMesh()->AddForce(
                ForceDirection * BallFollowForce * CurrentDistance // Scale force by distance
            );
        }
    }

}

// CORRECTED FUNCTION NAME
void AFieldPlayer::UpdateDecalVisibility(bool bIsPossessed)
{
    if (SelectionIndicator)
    {
        SelectionIndicator->SetVisibility(bIsPossessed);
    }
}

void AFieldPlayer::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (ABall* Ball = Cast<ABall>(OtherActor))
    {
        if (!AttachedBall)
        {
            AttachedBall = Ball;

            // Enable physics but disable gravity
            Ball->GetBallMesh()->SetEnableGravity(false);
            Ball->GetBallMesh()->SetLinearDamping(0.5f);
            Ball->GetBallMesh()->SetAngularDamping(0.5f);
        }
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

    if (AttachedBall)
    {
        const FVector KickDirection = GetActorForwardVector();
        AttachedBall->GetBallMesh()->AddForce(KickDirection * 50000 * AxisValue);
    }
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


// Remove duplicate OnBallHit implementation (keep only one)
void AFieldPlayer::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (ABall* Ball = Cast<ABall>(OtherActor))
    {
        if (!AttachedBall)
        {
            // Reset velocities first
            Ball->GetBallMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
            Ball->GetBallMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

            AttachedBall = Ball;

            // Configure physics
            Ball->GetBallMesh()->SetLinearDamping(2.0f);  // ?? Increased from 0.5
            Ball->GetBallMesh()->SetAngularDamping(2.0f); // ?? Increased from 0.5
            Ball->GetBallMesh()->SetEnableGravity(false);

            // Set collision response
            Ball->GetBallMesh()->SetCollisionResponseToChannel(
                ECC_Pawn,
                ECR_Ignore // ?? Prevent player collisions
            );
        }
    }
}