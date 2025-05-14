// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h" // Moved before .generated.h
#include "FieldPlayerAnim.h"
#include "FieldPlayer.generated.h"

UCLASS()
class IJAG_API AFieldPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    AFieldPlayer();


public:
    UFUNCTION(BlueprintCallable)
    bool HasBall() const { return bHasBall; }

    UFUNCTION(BlueprintCallable)
    void SetHasBall(bool bNewState) { bHasBall = bNewState; }

    



protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
    // Decal Component
    UPROPERTY(VisibleAnywhere, Category = "Possession")
    UDecalComponent* SelectionDecal;

    // Material Reference
    UPROPERTY(EditDefaultsOnly, Category = "Possession")
    UMaterialInterface* PossessedDecalMaterial;

    // Ball State
    UPROPERTY(VisibleAnywhere, Category = "Ball")
    bool bHasBall = false; // Declare ONCE

    void UpdateDecalVisibility(bool bIsPossessed);

    // Movement Properties
    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeedMultiplier = 1.5f;
};