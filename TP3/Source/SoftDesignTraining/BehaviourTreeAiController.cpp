// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"
#include "SDTFleeLocation.h"
#include "SoftDesignTraining.h"
#include "EngineUtils.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include <Kismet/GameplayStatics.h>

ABehaviourTreeAiController::ABehaviourTreeAiController()
    : m_targetPosBBKeyID(0),
    m_isTargetSeenBBKeyID(0)
{
    m_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

ABehaviourTreeAiController::~ABehaviourTreeAiController()
{

}

void ABehaviourTreeAiController::StartBehaviorTree(APawn* pawn)
{
    if (ASoftDesignTrainingCharacter* aiBaseCharacter = Cast<ASoftDesignTrainingCharacter>(pawn))
    {
        if (aiBaseCharacter->GetBehaviorTree())
        {
            m_behaviorTreeComponent->StartTree(*aiBaseCharacter->GetBehaviorTree());
        }
    }
}

void ABehaviourTreeAiController::StopBehaviorTree(APawn* pawn)
{
    if (ASoftDesignTrainingCharacter* aiBaseCharacter = Cast<ASoftDesignTrainingCharacter>(pawn))
    {
        if (aiBaseCharacter->GetBehaviorTree())
        {
            m_behaviorTreeComponent->StopTree();
        }
    }
}

void ABehaviourTreeAiController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);

    if (ASoftDesignTrainingCharacter* aiBaseCharacter = Cast<ASoftDesignTrainingCharacter>(pawn))
    {
        if (aiBaseCharacter->GetBehaviorTree())
        {
            m_blackboardComponent->InitializeBlackboard(*aiBaseCharacter->GetBehaviorTree()->BlackboardAsset);

            m_targetPosBBKeyID = m_blackboardComponent->GetKeyID("TargetPos");
            m_isTargetSeenBBKeyID = m_blackboardComponent->GetKeyID("TargetIsSeen");
            m_nextPatrolDestinationBBKeyID = m_blackboardComponent->GetKeyID("NextPatrolDest");
            m_currentPatrolDestinationBBKeyID = m_blackboardComponent->GetKeyID("CurrentPatrolDest");
            m_fleePosKeyID = m_blackboardComponent->GetKeyID("BestFleePos");

            //Set this agent in the BT
            m_blackboardComponent->SetValue<UBlackboardKeyType_Object>(m_blackboardComponent->GetKeyID("SelfActor"), pawn);
        }
    } 
}
FVector ABehaviourTreeAiController::GetBestFleeLocation() {
    float bestLocationScore = 0.f;
    ASDTFleeLocation* bestFleeLocation = nullptr;

    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter)
        return FVector::Zero();

    for (TActorIterator<ASDTFleeLocation> actorIterator(GetWorld(), ASDTFleeLocation::StaticClass()); actorIterator; ++actorIterator)
    {
        ASDTFleeLocation* fleeLocation = Cast<ASDTFleeLocation>(*actorIterator);
        if (fleeLocation)
        {
            float distToFleeLocation = FVector::Dist(fleeLocation->GetActorLocation(), playerCharacter->GetActorLocation());

            FVector selfToPlayer = playerCharacter->GetActorLocation() - GetPawn()->GetActorLocation();
            selfToPlayer.Normalize();

            FVector selfToFleeLocation = fleeLocation->GetActorLocation() - GetPawn()->GetActorLocation();
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

    if (bestFleeLocation) return bestFleeLocation->GetActorLocation();
    return FVector::Zero();
}
FHitResult ABehaviourTreeAiController::GetHighPriorityDetections() {
    // TO DO filtrer les collision pour player
    FHitResult hitResults;
    return hitResults;
}
