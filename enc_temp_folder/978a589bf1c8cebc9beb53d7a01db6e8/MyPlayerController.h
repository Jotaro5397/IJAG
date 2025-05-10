// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AFieldPlayer; // Forward declaration

UCLASS()
class IJAG_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void SetupInputComponent() override;
    UFUNCTION(BlueprintCallable)
    void SwitchPlayer();

private:
    TArray<AActor*> AllPlayers; // Single consistent declaration
    int32 CurrentPlayerIndex = 0;

    void CacheAllPlayers();
};
