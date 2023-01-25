// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "SDTAIController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
    GENERATED_BODY()
public:
    virtual void Tick(float deltaTime) override;
private:

    void WallAvoidance();
    bool DeathFloorAvoidance();
    void MoveTowardsPickup();
    bool IsInsideCone(APawn* pawn, AActor* targetActor) const;

    FVector m_speed;
    APawn* m_pawn;

    float m_accel = 300;
    float m_max_speed = 600;
    float m_wall_cast = 350;
    float m_side_cast = 200;
    bool m_initialized = false;
    float m_vision_angle = PI / 2.00f;
    float m_Collectebale_detection_reduis = 500;
};
