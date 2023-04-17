// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"
#include "SDTFleeLocation.h"
#include "SoftDesignTraining.h"
#include "SDTPathFollowingComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include <Kismet/GameplayStatics.h>
#include <SoftDesignTraining/SDTUtils.h>


ABehaviourTreeAiController::ABehaviourTreeAiController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USDTPathFollowingComponent>(TEXT("PathFollowingComponent"))),
    m_chaseLocationKeyID(0),
    m_fleeLocationKeyID(0),
    m_collectibleLocationKeyID(0),
    m_playerDetectedKeyID(0)
   
{
    UE_LOG(LogTemp, Log, TEXT("AgentCreated"));
    m_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

ABehaviourTreeAiController::~ABehaviourTreeAiController()
{
}
void ABehaviourTreeAiController::DebugCpuTime() {
    FString agentName = GetPawn()->GetName();
    float lastTotalTime = float(m_btCycleEnd- m_btCycleStart) * FPlatformTime::GetSecondsPerCycle();
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Agent: %s - Last behaviourTree time (total) : %f , LastPlayerDetectionTime: %f LastFleeLocationTime: %f , LastCollectibleTime: %f "), *agentName, lastTotalTime, m_DetectionTime, m_FleeTime, m_CollectibleTime));    
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

            m_chaseLocationKeyID = m_blackboardComponent->GetKeyID("ChaseLocation");
            m_fleeLocationKeyID = m_blackboardComponent->GetKeyID("FleeLocation");
            m_collectibleLocationKeyID = m_blackboardComponent->GetKeyID("CollectibleLocation");
            m_playerDetectedKeyID = m_blackboardComponent->GetKeyID("PlayerDetected");
           

            //Set this agent in the BT
            m_blackboardComponent->SetValue<UBlackboardKeyType_Object>(m_blackboardComponent->GetKeyID("SelfActor"), pawn);

            m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_chaseLocationKeyID, m_invalidLocation);
            m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_fleeLocationKeyID, m_invalidLocation);
            m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_collectibleLocationKeyID, m_invalidLocation);
            m_blackboardComponent->SetValue<UBlackboardKeyType_Bool>(m_playerDetectedKeyID, false);

           
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

    FVector detectionStartLocation = GetPawn()->GetActorLocation() + GetPawn()->GetActorForwardVector() * m_DetectionCapsuleForwardStartingOffset;
    FVector detectionEndLocation = detectionStartLocation + GetPawn()->GetActorForwardVector() * m_DetectionCapsuleHalfLength * 2;

    TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

    TArray<FHitResult> allDetectionHits;
    GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(m_DetectionCapsuleRadius));
    
    FHitResult outDetectionHit;

    for (const FHitResult& hit : allDetectionHits)
    {
        if (UPrimitiveComponent* component = hit.GetComponent())
        {
            if (component->GetCollisionObjectType() == COLLISION_PLAYER)
            {
                //we can't get more important than the player
                outDetectionHit = hit;
                return outDetectionHit; 
            }
            else if (component->GetCollisionObjectType() == COLLISION_COLLECTIBLE)
            {
                outDetectionHit = hit;
            }
        }
    }
    return outDetectionHit;
}
