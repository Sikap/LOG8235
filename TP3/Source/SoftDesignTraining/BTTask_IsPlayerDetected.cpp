// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsPlayerDetected.h"
#include "BehaviourTreeAiController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include <SoftDesignTraining/SDTUtils.h>


EBTNodeResult::Type UBTTask_IsPlayerDetected::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	if (ABehaviourTreeAiController* aiBtController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
	{
	
		if (OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>(aiBtController->GetPlayerDetectedKeyID()))
		{
			return EBTNodeResult::Succeeded;
		}

	}
	return EBTNodeResult::Failed;
}