// Fill out your copyright notice in the Description page of Project Settings.
#include "UBTTask_IsAgentJumping.h"
#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"


EBTNodeResult::Type UUBTTask_IsAgentJumping::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
    {
        if (ASoftDesignTrainingCharacter* aiBase = Cast<ASoftDesignTrainingCharacter>(aiController->GetCharacter()))
        {
            if (aiController->AtJumpSegment)
            {
                return EBTNodeResult::Succeeded;
            }
        }

    }

    return EBTNodeResult::Failed;
}
