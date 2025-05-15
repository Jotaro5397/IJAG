// ABroadCamera.h
// No changes needed here

// ABroadtCamera.cpp
#include "BroadCamera.h"
#include "FieldPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABroadCamera::ABroadCamera()
{
    PrimaryActorTick.bCanEverTick = true;


    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // No need to disable collision test unless you want the camera to clip through geometry
    SpringArmComponent->TargetArmLength = 500.0f; // Length of the Spring Arm, adjust as needed
    RootComponent = SpringArmComponent;

    // Initialize the Camera Component
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ABroadCamera::BeginPlay()
{
    Super::BeginPlay();

    // Access the player character (replace AFieldPlayer with your actual character class)
    AFieldPlayer* PlayerCharacter = Cast<AFieldPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (PlayerCharacter)
    {
        FVector InitialCameraLocation = PlayerCharacter->GetActorLocation() + CameraOffset;
        // CameraComponent->SetWorldLocation(InitialCameraLocation); // This line is problematic
        SetActorLocation(InitialCameraLocation); // You should move the Actor (which is the SpringArm's root)
    }
}

// ABroadcastCamera.cpp
void ABroadCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetBall)
    {
        FVector BallLocation = TargetBall->GetActorLocation();
        FVector NewCameraLocation = BallLocation + CameraOffset; // This is the target for the ACTOR
        FVector CurrentLocation = GetActorLocation();
        FVector InterpolatedLocation = FMath::VInterpTo(CurrentLocation, NewCameraLocation, DeltaTime, InterpSpeed);
        SetActorLocation(InterpolatedLocation); // This moves the SpringArm (RootComponent)
    }
}


void ABroadCamera::SetTargetBall(ABall* NewTargetBall)
{
    TargetBall = NewTargetBall;
}
