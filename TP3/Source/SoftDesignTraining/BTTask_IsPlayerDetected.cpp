// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsPlayerDetected.h"
#include "BehaviourTreeAiController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include <SoftDesignTraining/SDTUtils.h>


EBTNodeResult::Type UBTTask_IsPlayerDetected::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	if (ABehaviourTreeAiController* aiBtController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
	{
		FHitResult result = aiBtController->GetHighPriorityDetections();
		if (!result.GetComponent())return EBTNodeResult::Failed;
		if (result.GetComponent()->GetCollisionObjectType() != COLLISION_PLAYER) return EBTNodeResult::Failed;

		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiBtController->GetTargetPosBBKeyID(), result.GetActor()->GetActorLocation());

		return EBTNodeResult::Succeeded;

	}
	return EBTNodeResult::Failed;

}