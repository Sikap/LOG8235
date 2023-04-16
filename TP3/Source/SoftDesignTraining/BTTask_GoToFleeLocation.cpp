// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoToFleeLocation.h"
#include "BehaviourTreeAiController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UBTTask_GoToFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (ABehaviourTreeAiController* aiBtController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
		{


			return EBTNodeResult::Succeeded;

		}
	}

	return EBTNodeResult::Failed;
}