// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SDTCollectible.generated.h"

/**
 *
 */

UENUM()
enum ASDTCollectibleState {
    InitialState                UMETA(DisplayName = "InitialState"),
    LeftAcceleration             UMETA(DisplayName = "LeftAcceleration"),
    RightAcceleration        UMETA(DisplayName = "RightAcceleration"),
    CoolDown                UMETA(DisplayName = "CoolDown"),
};

UCLASS()
class SOFTDESIGNTRAINING_API ASDTCollectible : public AStaticMeshActor
{
    GENERATED_BODY()

public:
    ASDTCollectible();

    void Collect();
    void OnCooldownDone();
    bool IsOnCooldown();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
        float m_CollectCooldownDuration = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
        bool isMoveable = false;

    virtual void Tick(float deltaTime) override;
    virtual void BeginPlay() override;

    FVector initialPosition;

protected:
    FTimerHandle m_CollectCooldownTimer;
private:
    float speed;
    float max_speed = 5.f;
    float m_raycast = 1850.f;
    float acceleration;

    ASDTCollectibleState state = ASDTCollectibleState::InitialState;
};
