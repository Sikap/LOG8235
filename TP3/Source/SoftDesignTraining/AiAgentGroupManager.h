#pragma once
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "BehaviourTreeAiController.h"

class SOFTDESIGNTRAINING_API AAiAgentGroupManager 
{
public:
	// Sets default values for this empty's properties
	
    AAiAgentGroupManager();
    ~AAiAgentGroupManager();
	
	static AAiAgentGroupManager* GetInstance();
	static void Destroy();

	bool RegisterAIAgent(ABehaviourTreeAiController* aiAgent);
	bool HasAIAgent(ABehaviourTreeAiController* aiAgent);
	bool UnregisterAIAgent(ABehaviourTreeAiController* aiAgent);
	
	void AssignGroupPositions(UWorld* world);
    FVector GetAgentAssignedPosition(ABehaviourTreeAiController* agent);

public:
	//SINGLETON
	static AAiAgentGroupManager* m_Instance;

	TArray<ABehaviourTreeAiController*> m_registeredAgents;
	TMap<ABehaviourTreeAiController*, FVector> AgentAssignedPositions;
};

