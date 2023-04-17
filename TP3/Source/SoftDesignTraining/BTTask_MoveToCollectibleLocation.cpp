// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToCollectibleLocation.h"
#include "BehaviourTreeAiController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
EBTNodeResult::Type UBTTask_MoveToCollectibleLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


    if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
    {
        if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
        {
            FVector targetPosition = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(aiController->GetCollectibleLocationKeyID());
            float dist = targetPosition.Dist(targetPosition, aiController->GetPawn()->GetActorLocation());
            if (targetPosition == aiController->m_currentTarget )
            {
                //if(dist < 30){ return EBTNodeResult::Failed; }
                return EBTNodeResult::Succeeded;
            }

            if (targetPosition != aiController->m_currentTarget) {
                aiController->m_currentTarget = targetPosition;
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiController, targetPosition);
                return EBTNodeResult::Succeeded;
            }
           

        }
    }

    return EBTNodeResult::Failed;
}