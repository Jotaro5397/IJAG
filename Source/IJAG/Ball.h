// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ball.generated.h"

UCLASS()
class IJAG_API ABall : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABall();

public:
	UFUNCTION(BlueprintCallable)
	FVector GetBallLocation() const { return GetActorLocation(); }

	UPROPERTY(EditAnywhere, Category = "Ball Properties")
	float MoveSpeed = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
	UStaticMeshComponent* BallMesh;

	// Add this function
	UStaticMeshComponent* GetBallMesh() const { return BallMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
