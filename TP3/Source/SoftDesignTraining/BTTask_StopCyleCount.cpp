// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopCyleCount.h"

#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"


EBTNodeResult::Type UBTTask_StopCyleCount::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
    {
        aiController->m_btCycleEnd = FPlatformTime::Cycles();
        aiController->DebugCpuTime();

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}