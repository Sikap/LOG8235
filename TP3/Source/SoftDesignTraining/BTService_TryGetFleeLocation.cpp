// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TryGetFleeLocation.h"
#include <SoftDesignTraining/SDTFleeLocation.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviourTreeAiController.h"
#include "AiAgentGroupManager.h"
#include "EngineUtils.h"
#include <EngineMinimal.h>

UBTService_TryGetFleeLocation::UBTService_TryGetFleeLocation() {
	bCreateNodeInstance = true;
}

void UBTService_TryGetFleeLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const uint32 BroadcastBeginTime = FPlatformTime::Cycles();

    float bestLocationScore = 0.f;
    ASDTFleeLocation* bestFleeLocation = nullptr;

    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter)
        return;
   
    ABehaviourTreeAiController* aiController = Cast<ABehaviourTreeAiController>(OwnerComp.GetAIOwner());

    for (TActorIterator<ASDTFleeLocation> actorIterator(GetWorld(), ASDTFleeLocation::StaticClass()); actorIterator; ++actorIterator)
    {
        ASDTFleeLocation* fleeLocation = Cast<ASDTFleeLocation>(*actorIterator);
        if (fleeLocation)
        {
            float distToFleeLocation = FVector::Dist(fleeLocation->GetActorLocation(), playerCharacter->GetActorLocation());

            FVector selfToPlayer = playerCharacter->GetActorLocation() - aiController->GetPawn()->GetActorLocation();
            selfToPlayer.Normalize();

            FVector selfToFleeLocation = fleeLocation->GetActorLocation() - aiController->GetPawn()->GetActorLocation();
            selfToFleeLocation.Normalize();

            float fleeLocationToPlayerAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(selfToPlayer, selfToFleeLocation)));
            float locationScore = distToFleeLocation + fleeLocationToPlayerAngle * 100.f;

            if (locationScore > bestLocationScore)
            {
                bestLocationScore = locationScore;
                bestFleeLocation = fleeLocation;
            }

            DrawDebugString(GetWorld(), FVector(0.f, 0.f, 10.f), FString::SanitizeFloat(locationScore), fleeLocation, FColor::Red, 5.f, false);
        }
    }
    //Set this agent in the BT
    if (bestFleeLocation) {
        aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetFleeLocationBBKeyID(), bestFleeLocation->GetActorLocation());
        
        AAiAgentGroupManager* aiAgentGroupManager = AAiAgentGroupManager::GetInstance();
        aiAgentGroupManager->UnregisterAIAgent(aiController);
    }
    else {
        aiController->m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(aiController->GetFleeLocationBBKeyID(), aiController->m_invalidLocation);
    }

    const uint32 BroadcastEndTime = FPlatformTime::Cycles();
    aiController->m_FleeTime = FPlatformTime::GetSecondsPerCycle() * float(BroadcastEndTime - BroadcastBeginTime);

}