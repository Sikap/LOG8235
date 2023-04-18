// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceTryDetectPlayer.h"
#include "SDTUtils.h"
#include "EngineUtils.h"
#include "BehaviourTreeAiController.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"

UBTServiceTryDetectPlayer::UBTServiceTryDetectPlayer() {
	bCreateNodeInstance = true;
}

void UBTServiceTryDetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	

	const uint32 BroadcastBeginTime = FPlatformTime::Cycles();
	

	ABehaviourTreeAiController* aiBtController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner());
	if (aiBtController)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiBtController->GetPlayerDetectedKeyID(), false);

		FHitResult result = aiBtController->GetHighPriorityDetections();
		if (result.GetComponent() && result.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER)
		{
			ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (!playerCharacter) {
				const uint32 BroadcastEndTime = FPlatformTime::Cycles();
				aiBtController->m_DetectionTime = FPlatformTime::GetSecondsPerCycle() * float(BroadcastEndTime - BroadcastBeginTime);
				return;
			}

			TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
			TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

			FHitResult losHit;
			GetWorld()->LineTraceSingleByObjectType(losHit, aiBtController->GetPawn()->GetActorLocation(), playerCharacter->GetActorLocation(), TraceObjectTypes);
			if (losHit.GetComponent() && losHit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiBtController->GetChaseLocationKeyID(), playerCharacter->GetActorLocation());
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiBtController->GetPlayerDetectedKeyID(), true);
			}
		}
	}
	
	
	const uint32 BroadcastEndTime = FPlatformTime::Cycles();
	aiBtController->m_DetectionTime = FPlatformTime::GetSecondsPerCycle() * float(BroadcastEndTime - BroadcastBeginTime);

}