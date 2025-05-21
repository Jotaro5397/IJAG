// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "FieldPlayerAnim.h"
#include "FieldPlayer.generated.h"

class ABall; 

UCLASS()
class IJAG_API AFieldPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    AFieldPlayer();


public:
    // Ball Properties

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
    ABall* AttachedBall;

    UFUNCTION(BlueprintCallable)
    FVector GetBallLocation() const;

    // Adjust these properties
    UPROPERTY(EditAnywhere, Category = "Ball", meta = (ClampMin = "0.0"))
    float BallFollowForce = 50000.0f; // Increased from negative value

    UPROPERTY(EditAnywhere, Category = "Ball", meta = (ClampMin = "0.0"))
    float BallDistance = 200.0f; // Positive distance value

    UPROPERTY(EditAnywhere, Category = "Ball")
    float PositionTolerance = 50.0f;

    UFUNCTION()
    void OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    UFUNCTION(BlueprintCallable)
    bool HasBall() const { return bHasBall; }


public:
    
    UFUNCTION(BlueprintCallable)
    void SetHasBall(bool bNewState) { bHasBall = bNewState; }


    void UpdateDecalVisibility(bool bIsPossessed);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FRotator LastKnownRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicator")
    FVector IndicatorOffset = FVector(0.f, 0.f, -90.f); // Default offset (adjust Z for feet)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicator")
    FVector IndicatorScale = FVector(1.f); // Default scale

protected:

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;


    UFUNCTION(BlueprintCallable)
    void TriggerPassAnimation();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    UFieldPlayerAnim* AnimInstance;



private:
    // Add timer handler declaration
    FTimerHandle PassAnimationTimer;

    // Add callback declaration
    void OnPassAnimationCompleted();

    // Movement functions
    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void Sprint();
    void StopSprinting();

private:
  
    UPROPERTY(VisibleAnywhere, Category = "Possession")
    UStaticMeshComponent* SelectionIndicator;

    // Ball State
    UPROPERTY(VisibleAnywhere, Category = "Ball")
    bool bHasBall = false; // Declare ONCE

    // Movement Properties
    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeedMultiplier = 1.5f;
};