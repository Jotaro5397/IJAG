// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AFieldPlayer;

UCLASS()
class IJAG_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMyPlayerController();
    virtual void BeginPlay() override;
    void SetBroadCameraAsViewTarget();
    virtual void SetupInputComponent() override;
    

    UFUNCTION(BlueprintCallable)
    void SwitchPlayer();

    virtual void OnPossess(APawn* InPawn) override;


private:
    TArray<AActor*> AllPlayers;
    int32 CurrentPlayerIndex = 0;

    void CacheAllPlayers();
    
    void PossessPlayerAndSetView();
};