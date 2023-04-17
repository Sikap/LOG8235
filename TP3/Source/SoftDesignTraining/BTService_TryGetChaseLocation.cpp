// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TryGetChaseLocation.h"
#include "BehaviourTreeAiController.h"
#include "EngineUtils.h"
#include <EngineMinimal.h>
#include "AiAgentGroupManager.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
UBTService_TryGetChaseLocation::UBTService_TryGetChaseLocation() {
	bCreateNodeInstance = true;
}

void UBTService_TryGetChaseLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner());

    AAiAgentGroupManager* aiAgentGroupManager = AAiAgentGroupManager::GetInstance();
    if (aiAgentGroupManager)
    {
        FVector assignedPosition = aiAgentGroupManager->GetAgentAssignedPosition(aiController);
        if (!assignedPosition.IsZero()) {
            aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetChaseLocationKeyID(), assignedPosition);
        }
        else {
            aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetChaseLocationKeyID(), aiController->m_invalidLocation);
        }
    }
}
