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
            aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetChaseLocationKeyID(), aiController->m_invalidLocation);
            aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetFleeLocationBBKeyID(), aiController->m_invalidLocation);

            FVector targetPosition = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(aiController->GetCollectibleLocationKeyID());
            float dist = targetPosition.Dist(targetPosition, aiController->GetPawn()->GetActorLocation());
            if (targetPosition == aiController->m_currentTarget )
            {
                //if(dist < 30){ return EBTNodeResult::Failed; }
                return EBTNodeResult::Succeeded;
            }

            if (targetPosition != aiController->m_currentTarget) {
                aiController->m_currentTarget = targetPosition;
               // UAIBlueprintHelperLibrary::SimpleMoveToLocation(aiController, targetPosition);
                aiController->MoveToLocation(targetPosition, 0.5f, false, true, true, NULL, false);
                return EBTNodeResult::Succeeded;
            }
           

        }
    }

    return EBTNodeResult::Failed;
}