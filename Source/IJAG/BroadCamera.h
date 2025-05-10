// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Ball.h"
#include "BroadCamera.generated.h"

UCLASS()
class IJAG_API ABroadCamera : public AActor
{
    GENERATED_BODY()

public:
    ABroadCamera();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float InterpSpeed = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    FVector CameraOffset = FVector(-695.5f, -21.2f, 377.4f);

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* CameraComponent;

    virtual void Tick(float DeltaTime) override;
    void SetTargetBall(ABall* NewTargetBall);

protected:
    virtual void BeginPlay() override;

private:
    ABall* TargetBall;
};
