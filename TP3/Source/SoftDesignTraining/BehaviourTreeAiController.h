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
	ABehaviourTreeAiController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~ABehaviourTreeAiController();

	UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return m_behaviorTreeComponent; }
	UBlackboardComponent* GetBlackBoardComponent() const { return m_blackboardComponent; }

	uint8					GetFleeLocationBBKeyID() const { return m_fleeLocationKeyID; }
	uint8                   GetChaseLocationKeyID() const { return m_chaseLocationKeyID; }
	//uint8                   GetNextPatrolDestinationKeyID() const { return m_nextPatrolDestinationBBKeyID; }
	//uint8                   GetCurrentPatrolDestinationKeyID() const { return m_currentPatrolDestinationBBKeyID; }

	void                    StartBehaviorTree(APawn* pawn);
	void                    StopBehaviorTree(APawn* pawn);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float m_DetectionCapsuleHalfLength = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float m_DetectionCapsuleRadius = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float m_DetectionCapsuleForwardStartingOffset = 100.f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		UCurveFloat* JumpCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float JumpApexHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float JumpSpeed = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		bool AtJumpSegment = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		bool InAir = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		bool Landing = false;

protected:
	virtual void OnPossess(APawn* pawn) override;
public:
	FVector GetBestFleeLocation();
	uint8 GetFleePosBBKeyID() const { return m_fleeLocationKeyID; }
	FHitResult GetHighPriorityDetections();
	UPROPERTY(transient)
		UBehaviorTreeComponent* m_behaviorTreeComponent;
	UPROPERTY(transient)
		UBlackboardComponent* m_blackboardComponent;

	FVector m_currentTarget;
	FVector m_invalidLocation = FVector(0, 0, -9999);
private:





	uint8   m_chaseLocationKeyID;
	uint8   m_fleeLocationKeyID;
	uint8   m_collectibleLocationKeyID;

	
};
