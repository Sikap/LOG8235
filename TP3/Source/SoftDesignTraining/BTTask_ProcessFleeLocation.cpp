// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ProcessFleeLocation.h"
#include "BehaviourTreeAiController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UBTTask_ProcessFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (ABehaviourTreeAiController* aiBtController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner()))
		{
			// CPU Usage time
			double startTime = FPlatformTime::Seconds();
			FVector fleePos = aiBtController->GetBestFleeLocation();

			//end timer 
			double TimeElapsedInS = FPlatformTime::Seconds() - startTime;
			DrawDebugString(GetWorld(), FVector(0.f, 0.f, 8.f), "MoveToFlee: " + FString::SanitizeFloat(TimeElapsedInS) + "s", aiBtController->GetPawn(), FColor::Red, .5f, false);

			if (fleePos == FVector::Zero()) return EBTNodeResult::Failed;
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiBtController->GetFleePosBBKeyID(), fleePos);

			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}