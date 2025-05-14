// Fill out your copyright notice in the Description page of Project Settings.

// MyGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPlayerController.h" 
#include "BroadCamera.h"
#include "MyGameMode.generated.h"

class AMyPlayerController;
class ABroadCamera; // Correct class name
class ABall;

UCLASS()
class IJAG_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyGameMode();

protected:
    virtual void BeginPlay() override;

private:
    void InitializeCameraAndBall();

    // CORRECTED: Use ABroadCamera instead of ABroadcastCamera
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    TSubclassOf<ABroadCamera> BroadcastCameraClass;

    UPROPERTY(EditDefaultsOnly, Category = "Ball")
    TSubclassOf<ABall> BallClass;
};