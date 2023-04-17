// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToFleeLocation.h"
#include "BehaviourTreeAiController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UBTTask_MoveToFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


    if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
    {
        if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
        {
            FVector targetPosition = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(aiController->GetFleeLocationBBKeyID());
            if (targetPosition == aiController->m_currentTarget)
            {
                return EBTNodeResult::Succeeded;
            }
            
            aiController->m_currentTarget = targetPosition;
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiController, targetPosition);
            return EBTNodeResult::Succeeded;
            
        }
    }

    return EBTNodeResult::Failed;
}