// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviourTreeAiController.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API ABehaviourTreeAiController : public AAIController
{
	GENERATED_BODY()
public:
	ABehaviourTreeAiController();
	~ABehaviourTreeAiController();

	UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return m_behaviorTreeComponent; }
	UBlackboardComponent* GetBlackBoardComponent() const { return m_blackboardComponent; }

	uint8                   GetTargetPosBBKeyID() const { return m_targetPosBBKeyID; }
	uint8                   GetTargetSeenKeyID() const { return m_isTargetSeenBBKeyID; }
	uint8                   GetNextPatrolDestinationKeyID() const { return m_nextPatrolDestinationBBKeyID; }
	uint8                   GetCurrentPatrolDestinationKeyID() const { return m_currentPatrolDestinationBBKeyID; }

	void                    StartBehaviorTree(APawn* pawn);
	void                    StopBehaviorTree(APawn* pawn);

protected:
	virtual void OnPossess(APawn* pawn) override;
private:

	UPROPERTY(transient)
		UBehaviorTreeComponent* m_behaviorTreeComponent;

	UPROPERTY(transient)
		UBlackboardComponent* m_blackboardComponent;


	uint8   m_targetPosBBKeyID;
	uint8   m_isTargetSeenBBKeyID;
	uint8   m_nextPatrolDestinationBBKeyID;
	uint8   m_currentPatrolDestinationBBKeyID;
	
};
