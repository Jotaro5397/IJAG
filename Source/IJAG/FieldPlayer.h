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

    UFUNCTION(BlueprintCallable)
    void BecomePossessed(APlayerController* NewController);

    UFUNCTION(BlueprintCallable)
    void LosePossession();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable)
    void TriggerPassAnimation();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
    UFieldPlayerAnim* AnimInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Possession")
    UDecalComponent* SelectionDecal;

    UFUNCTION(BlueprintCallable)
    void SetPossessionIndicator(bool bIsPossessed);

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
    // Movement properties
    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeedMultiplier = 1.5f;
};