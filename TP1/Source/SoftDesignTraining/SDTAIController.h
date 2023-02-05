// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "SDTAIController.generated.h"

/**
 * 
 */


UENUM()
enum ASDTAIState {
    Idle                UMETA(DisplayName = "Idle"),
    Roaming             UMETA(DisplayName = "Roaming"),
    GoingForPickup      UMETA(DisplayName = "GoingForPickup"),
    Attacking           UMETA(DisplayName = "Attacking"),
    Fleeing             UMETA(DisplayName = "Fleeing"),
};

UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
    GENERATED_BODY()
public:
    virtual void Tick(float deltaTime) override; 
    virtual void OnPossess(APawn* p) override;
    // Max speed of the Agent
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_max_speed = 300;
    // Acceleration of the Agent
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_acceleration = 425;
    // Agent detection reduis of Collectables 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_Collectebale_detection_reduis = 500;
    // Buffer space between Agent and wall
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
     float m_wall_cast = 200;
    // Buffer space between Agent and death wall
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
     float m_side_cast = 100;
    // Feild vision of Agent
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
     float m_vision_angle = PI / 2.00f;

private:

    const FVector UNDEFINED_TARGET_POINT = FVector(-1, -1, -1);
    FVector runIdleState();
    void WallAvoidance();
    bool DeathFloorAvoidance();
    FVector MoveTowardsPickup();
    FVector PlayerDetection();
    bool IsInsideCone(FVector targetActor) const;
    bool MoveToTargetPoint(float deltatime);
    void checkParameterValidity();

    FVector m_speed;
    FVector m_targetPoint = UNDEFINED_TARGET_POINT;
    APawn* m_pawn;
    ASDTAIState state = ASDTAIState::Idle;
    bool m_initialized = false;
};
