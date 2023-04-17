// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FoundFleeLocation.h"
#include "BehaviourTreeAiController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
EBTNodeResult::Type UBTTask_FoundFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
      if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
    {
        if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
        {
            FVector targetPosition = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(aiController->GetFleeLocationBBKeyID());
            //Agent is moving to target
            if (targetPosition == aiController->m_invalidLocation)
            {
                return EBTNodeResult::Failed;
            }
        }
    }
    return EBTNodeResult::Succeeded;
    

}