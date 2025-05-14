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

    // If there is a valid PlayerCharacter, initialize the camera's position based on the character's location plus the offset
    if (PlayerCharacter)
    {
        FVector InitialCameraLocation = PlayerCharacter->GetActorLocation() + CameraOffset;
        CameraComponent->SetWorldLocation(InitialCameraLocation);
    }
}

// ABroadcastCamera.cpp
void ABroadCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetBall)
    {
        FVector BallLocation = TargetBall->GetActorLocation();
        UE_LOG(LogTemp, Warning, TEXT("Ball Location: %s"), *BallLocation.ToString());

        // Use CameraOffset directly, which now includes the Z (height) component
        FVector NewCameraLocation = BallLocation + CameraOffset;

        UE_LOG(LogTemp, Warning, TEXT("New Camera Location Before Interpolation: %s"), *NewCameraLocation.ToString());

        // Interpolate the movement for smoother following
        FVector CurrentLocation = GetActorLocation();
        FVector InterpolatedLocation = FMath::VInterpTo(CurrentLocation, NewCameraLocation, DeltaTime, InterpSpeed);

        SetActorLocation(InterpolatedLocation);

        UE_LOG(LogTemp, Warning, TEXT("Interpolated Camera Location: %s"), *InterpolatedLocation.ToString());
    }
}


void ABroadCamera::SetTargetBall(ABall* NewTargetBall)
{
    TargetBall = NewTargetBall;
}
