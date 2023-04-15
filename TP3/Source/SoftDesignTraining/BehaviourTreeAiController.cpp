// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTreeAiController.h"
#include "SoftDesignTrainingCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

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

            //Set this agent in the BT
            m_blackboardComponent->SetValue<UBlackboardKeyType_Object>(m_blackboardComponent->GetKeyID("SelfActor"), pawn);
        }
    }
}