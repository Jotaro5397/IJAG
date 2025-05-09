// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FieldPlayerAnim.generated.h"

UCLASS()
class IJAG_API UFieldPlayerAnim : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // Animation parameters
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float Speed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float Direction;

    // Pass animation control
    UFUNCTION(BlueprintCallable)
    void StartPassAnimation();

    UFUNCTION(BlueprintCallable)
    void StopPassAnimation();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    bool bIsPassing;
};