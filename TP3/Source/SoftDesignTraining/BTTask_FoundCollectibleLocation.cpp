// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FoundCollectibleLocation.h"

#include "BehaviourTreeAiController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
EBTNodeResult::Type UBTTask_FoundCollectibleLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


    if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
    {
        if (ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
        {
            FVector targetPosition = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(aiController->GetCollectibleLocationKeyID());
            
            if (targetPosition == aiController->m_invalidLocation)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("FAILED")));
                return EBTNodeResult::Failed;
            }
        }
    }
    return EBTNodeResult::Succeeded;

}