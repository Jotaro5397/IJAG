// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldPlayer.h"
#include "Components/DecalComponent.h"
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

    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
    SelectionDecal->SetupAttachment(RootComponent);
    SelectionDecal->SetDecalMaterial(LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Path/To/DefaultDecalMaterial")));
    SelectionDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    SelectionDecal->DecalSize = FVector(64.f, 128.f, 128.f);
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

void AFieldPlayer::SetPossessionIndicator(bool bIsPossessed)
{
    SelectionDecal->SetVisibility(bIsPossessed);
    
    // Create dynamic material instance with proper UE5.5 syntax
    if (UMaterialInterface* BaseMaterial = SelectionDecal->GetDecalMaterial())
    {
        UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        SelectionDecal->SetDecalMaterial(DynMaterial);
        
        if (DynMaterial)
        {
            FLinearColor Color = bIsPossessed ? FLinearColor::Green : FLinearColor::Red;
            DynMaterial->SetVectorParameterValue("Color", Color);
        }
    }
}

void AFieldPlayer::BecomePossessed(APlayerController* NewController)
{
    SetPossessionIndicator(true);
    PossessedBy(NewController);
}

void AFieldPlayer::LosePossession()
{
    SetPossessionIndicator(false);
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
    // Use controller rotation for proper camera-relative movement
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    AddMovementInput(Direction, AxisValue);
}

void AFieldPlayer::MoveRight(float AxisValue)
{
    // Use controller rotation for proper camera-relative movement
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
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