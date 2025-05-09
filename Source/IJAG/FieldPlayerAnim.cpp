// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldPlayerAnim.h"
#include "FieldPlayer.h"
#include "KismetAnimationLibrary.h"

void UFieldPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (const APawn* Pawn = TryGetPawnOwner())
    {
        Speed = Pawn->GetVelocity().Size2D();
        Direction = UKismetAnimationLibrary::CalculateDirection(
            Pawn->GetVelocity(),
            Pawn->GetActorRotation()
        );
    }
}

void UFieldPlayerAnim::StartPassAnimation()
{
    bIsPassing = true;
}

void UFieldPlayerAnim::StopPassAnimation()
{
    bIsPassing = false;
}