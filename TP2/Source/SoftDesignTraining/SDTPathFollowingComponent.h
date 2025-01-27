// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "SDTAIController.h"
#include "SDTPathFollowingComponent.generated.h"

/**
*
*/
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API USDTPathFollowingComponent : public UPathFollowingComponent
{
    GENERATED_UCLASS_BODY()

public:
    ASDTAIController* controllerRef; 
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    float timeToTravel;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    float currentJumpT;
    
    float initSpeed;
    FVector basePlayerLocation;
    virtual void FollowPathSegment(float deltaTime) override;
    virtual void SetMoveSegment(int32 segmentStartIndex) override;
};