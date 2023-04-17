// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartCyleCount.h"

#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"


EBTNodeResult::Type UBTTask_StartCyleCount::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
    {
        aiController->m_btCycleStart = FPlatformTime::Cycles();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}