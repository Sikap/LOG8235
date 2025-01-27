// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AgentTimeLogger.h"
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
	uint8                   GetCollectibleLocationKeyID() const { return m_collectibleLocationKeyID; }
	uint8                   GetPlayerDetectedKeyID() const { return m_playerDetectedKeyID; }

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
	
	FHitResult GetHighPriorityDetections();


	void DebugCpuTime();

	UPROPERTY(transient)
		UBehaviorTreeComponent* m_behaviorTreeComponent;
	UPROPERTY(transient)
		UBlackboardComponent* m_blackboardComponent;

	FVector m_currentTarget;
	FVector m_invalidLocation = FVector(0, 0, -9999);

	int m_debugKey = -1;

	float m_FleeTime = 0;
	float m_DetectionTime = 0;
	float m_CollectibleTime = 0;

	uint32 m_btCycleStart = 0;
	uint32 m_btCycleEnd = 0;

	AAgentTimeLogger* m_loggerRef = nullptr;
private:



	virtual void Tick(float deltaTime) override;

	uint8   m_chaseLocationKeyID;
	uint8   m_fleeLocationKeyID;
	uint8   m_collectibleLocationKeyID;
	uint8   m_playerDetectedKeyID;

	
};
