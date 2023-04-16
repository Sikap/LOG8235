// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_IsPlayerPoweredUp.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "SDTUtils.h"
#include "BehaviourTreeAiController.h"

EBTNodeResult::Type UBTTask_IsPlayerPoweredUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (ABehaviourTreeAiController* aiBtController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner())) {

        if (SDTUtils::IsPlayerPoweredUp(GetWorld())) {
            return EBTNodeResult::Succeeded;
        }
    }
        return EBTNodeResult::Failed;

}
