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
    virtual void BeginPlay() override;
    void SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime);
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable)
    void SwitchPlayer();

private:
    TArray<AActor*> AllPlayers;
    int32 CurrentPlayerIndex = 0;

    void CacheAllPlayers();
};