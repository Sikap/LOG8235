// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToChaseLocation.h"
#include "BehaviourTreeAiController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
EBTNodeResult::Type UBTTask_MoveToChaseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


    if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
    {
        if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
        {
            FVector targetPosition = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(aiController->GetChaseLocationKeyID());
            if (targetPosition == aiController->m_currentTarget)
            {
                return EBTNodeResult::Succeeded;
            }

            aiController->m_currentTarget = targetPosition;
            aiController->MoveToLocation(targetPosition, 0.5f, false, true, true, NULL, false);
            return EBTNodeResult::Succeeded;

        }
    }

    return EBTNodeResult::Failed;
}